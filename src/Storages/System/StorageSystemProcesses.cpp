/*
 * Copyright 2016-2023 ClickHouse, Inc.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


/*
 * This file may have been modified by Bytedance Ltd. and/or its affiliates (“ Bytedance's Modifications”).
 * All Bytedance's Modifications are Copyright (2023) Bytedance Ltd. and/or its affiliates.
 */

#include <DataTypes/DataTypeString.h>
#include <DataTypes/DataTypesNumber.h>
#include <DataTypes/DataTypeArray.h>
#include <DataTypes/DataTypeMap.h>
#include <DataTypes/DataTypeMap.h>
#include <DataTypes/DataTypeFactory.h>
#include <Interpreters/ProcessList.h>
#include <Storages/System/StorageSystemProcesses.h>
#include <Parsers/formatTenantDatabaseName.h>
#include <Interpreters/Context.h>
#include <Core/Settings.h>
#include <Interpreters/ProfileEventsExt.h>
#include <Common/typeid_cast.h>
#include <Common/IPv6ToBinary.h>
#include <Columns/ColumnsNumber.h>
#include <Columns/ColumnArray.h>


namespace DB
{

NamesAndTypesList StorageSystemProcesses::getNamesAndTypes()
{
    return {
        {"is_initial_query", std::make_shared<DataTypeUInt8>()},

        {"user", std::make_shared<DataTypeString>()},
        {"query_id", std::make_shared<DataTypeString>()},
        {"address", DataTypeFactory::instance().get("IPv6")},
        {"port", std::make_shared<DataTypeUInt16>()},

        {"initial_user", std::make_shared<DataTypeString>()},
        {"initial_query_id", std::make_shared<DataTypeString>()},
        {"initial_address", DataTypeFactory::instance().get("IPv6")},
        {"initial_port", std::make_shared<DataTypeUInt16>()},

        {"interface", std::make_shared<DataTypeUInt8>()},

        {"os_user", std::make_shared<DataTypeString>()},
        {"client_hostname", std::make_shared<DataTypeString>()},
        {"client_name", std::make_shared<DataTypeString>()},
        {"client_revision", std::make_shared<DataTypeUInt64>()},
        {"client_version_major", std::make_shared<DataTypeUInt64>()},
        {"client_version_minor", std::make_shared<DataTypeUInt64>()},
        {"client_version_patch", std::make_shared<DataTypeUInt64>()},

        {"http_method", std::make_shared<DataTypeUInt8>()},
        {"http_user_agent", std::make_shared<DataTypeString>()},
        {"http_referer", std::make_shared<DataTypeString>()},
        {"forwarded_for", std::make_shared<DataTypeString>()},

        {"quota_key", std::make_shared<DataTypeString>()},

        {"elapsed", std::make_shared<DataTypeFloat64>()},
        {"is_cancelled", std::make_shared<DataTypeUInt8>()},
        {"read_rows", std::make_shared<DataTypeUInt64>()},
        {"read_bytes", std::make_shared<DataTypeUInt64>()},
        {"total_rows_approx", std::make_shared<DataTypeUInt64>()},
        {"written_rows", std::make_shared<DataTypeUInt64>()},
        {"written_bytes", std::make_shared<DataTypeUInt64>()},
        {"memory_usage", std::make_shared<DataTypeInt64>()},
        {"peak_memory_usage", std::make_shared<DataTypeInt64>()},
        {"query", std::make_shared<DataTypeString>()},

        {"thread_ids", std::make_shared<DataTypeArray>(std::make_shared<DataTypeUInt64>())},

        {"ProfileEvents", std::make_shared<DataTypeMap>(std::make_shared<DataTypeString>(), std::make_shared<DataTypeUInt64>())},
        {"Settings", std::make_shared<DataTypeMap>(std::make_shared<DataTypeString>(), std::make_shared<DataTypeString>())},

        {"current_database", std::make_shared<DataTypeString>()},
    };
}

NamesAndAliases StorageSystemProcesses::getNamesAndAliases()
{
    return
    {
        {"ProfileEvents.Names", {std::make_shared<DataTypeArray>(std::make_shared<DataTypeString>())}, "mapKeys(ProfileEvents)"},
        {"ProfileEvents.Values", {std::make_shared<DataTypeArray>(std::make_shared<DataTypeUInt64>())}, "mapValues(ProfileEvents)"},
        {"Settings.Names", {std::make_shared<DataTypeArray>(std::make_shared<DataTypeString>())}, "mapKeys(Settings)" },
        {"Settings.Values", {std::make_shared<DataTypeArray>(std::make_shared<DataTypeString>())}, "mapValues(Settings)"}
    };
}

void StorageSystemProcesses::fillData(MutableColumns & res_columns, ContextPtr context, const SelectQueryInfo &) const
{
    ProcessList::Info info = context->getProcessList().getInfo(true, true, true);
    auto tenant_id = getCurrentTenantId();
    if (!context->is_tenant_user())
        tenant_id.resize(0);

    for (const auto & process : info)
    {
        if (!tenant_id.empty() && !isTenantMatchedEntityName(process.client_info.initial_user, tenant_id))
        {
            continue;
        }

        size_t i = 0;

        res_columns[i++]->insert(process.client_info.query_kind == ClientInfo::QueryKind::INITIAL_QUERY);

        res_columns[i++]->insert(getOriginalEntityName(process.client_info.current_user, tenant_id));
        res_columns[i++]->insert(process.client_info.current_query_id);
        res_columns[i++]->insertData(IPv6ToBinary(process.client_info.current_address.host()).data(), 16);
        res_columns[i++]->insert(process.client_info.current_address.port());

        res_columns[i++]->insert(getOriginalEntityName(process.client_info.initial_user, tenant_id));
        res_columns[i++]->insert(process.client_info.initial_query_id);
        res_columns[i++]->insertData(IPv6ToBinary(process.client_info.initial_address.host()).data(), 16);
        res_columns[i++]->insert(process.client_info.initial_address.port());

        res_columns[i++]->insert(UInt64(process.client_info.interface));

        res_columns[i++]->insert(process.client_info.os_user);
        res_columns[i++]->insert(process.client_info.client_hostname);
        res_columns[i++]->insert(process.client_info.client_name);
        res_columns[i++]->insert(process.client_info.client_tcp_protocol_version);
        res_columns[i++]->insert(process.client_info.client_version_major);
        res_columns[i++]->insert(process.client_info.client_version_minor);
        res_columns[i++]->insert(process.client_info.client_version_patch);

        res_columns[i++]->insert(UInt64(process.client_info.http_method));
        res_columns[i++]->insert(process.client_info.http_user_agent);
        res_columns[i++]->insert(process.client_info.http_referer);
        res_columns[i++]->insert(process.client_info.forwarded_for);

        res_columns[i++]->insert(process.client_info.quota_key);

        res_columns[i++]->insert(static_cast<double>(process.elapsed_microseconds) / 1000000.0);
        res_columns[i++]->insert(process.is_cancelled);
        res_columns[i++]->insert(process.read_rows);
        res_columns[i++]->insert(process.read_bytes);
        res_columns[i++]->insert(process.total_rows);
        res_columns[i++]->insert(process.written_rows);
        res_columns[i++]->insert(process.written_bytes);
        res_columns[i++]->insert(process.memory_usage);
        res_columns[i++]->insert(process.peak_memory_usage);
        res_columns[i++]->insert(process.query);

        {
            Array threads_array;
            threads_array.reserve(process.thread_ids.size());
            for (const UInt64 thread_id : process.thread_ids)
                threads_array.emplace_back(thread_id);
            res_columns[i++]->insert(threads_array);
        }

        {
            IColumn * column = res_columns[i++].get();

            if (process.profile_counters)
                ProfileEvents::dumpToMapColumn(*process.profile_counters, column, true);
            else
            {
                column->insertDefault();
            }
        }

        {
            IColumn * column = res_columns[i++].get();

            if (process.query_settings)
                process.query_settings->dumpToMapColumn(column, true);
            else
            {
                column->insertDefault();
            }
        }

        res_columns[i++]->insert(getOriginalEntityName(process.current_database, tenant_id));
    }
}

}
