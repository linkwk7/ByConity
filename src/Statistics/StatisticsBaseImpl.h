/*
 * Copyright (2022) Bytedance Ltd. and/or its affiliates
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once
#include <Statistics/StatisticsBase.h>
#include <Common/Exception.h>
#include <Statistics/SerdeUtils.h>

namespace DB::Statistics
{
template <class StatsDerived>
inline void checkTag(StatisticsTag tag)
{
    if (StatsDerived::tag != tag)
    {
        throw Exception("Statistics Tag mismatch", ErrorCodes::TYPE_MISMATCH);
    }
}

template <class StatsType>
std::shared_ptr<StatsType> createStatisticsTyped(StatisticsTag tag, std::string_view blob);

template <class StatsType>
std::shared_ptr<StatsType> createStatisticsUntyped(StatisticsTag tag, std::string_view blob);

}
