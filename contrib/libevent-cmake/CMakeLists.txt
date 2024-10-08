set (LIBRARY_DIR "${ClickHouse_SOURCE_DIR}/contrib/libevent")

set (SRCS
"${CMAKE_CURRENT_SOURCE_DIR}/include/event2/event-config.h"
"${CMAKE_CURRENT_SOURCE_DIR}/include/evconfig-private.h"
"${LIBRARY_DIR}/bufferevent-internal.h"
"${LIBRARY_DIR}/changelist-internal.h"
"${LIBRARY_DIR}/defer-internal.h"
"${LIBRARY_DIR}/epolltable-internal.h"
"${LIBRARY_DIR}/evbuffer-internal.h"
"${LIBRARY_DIR}/event-internal.h"
"${LIBRARY_DIR}/evmap-internal.h"
"${LIBRARY_DIR}/evrpc-internal.h"
"${LIBRARY_DIR}/evsignal-internal.h"
"${LIBRARY_DIR}/evthread-internal.h"
"${LIBRARY_DIR}/ht-internal.h"
"${LIBRARY_DIR}/http-internal.h"
"${LIBRARY_DIR}/iocp-internal.h"
"${LIBRARY_DIR}/ipv6-internal.h"
"${LIBRARY_DIR}/log-internal.h"
"${LIBRARY_DIR}/minheap-internal.h"
"${LIBRARY_DIR}/mm-internal.h"
"${LIBRARY_DIR}/ratelim-internal.h"
"${LIBRARY_DIR}/strlcpy-internal.h"
"${LIBRARY_DIR}/util-internal.h"
"${LIBRARY_DIR}/compat/sys/queue.h"
"${LIBRARY_DIR}/include/event2/buffer.h"
"${LIBRARY_DIR}/include/event2/bufferevent.h"
"${LIBRARY_DIR}/include/event2/bufferevent_compat.h"
"${LIBRARY_DIR}/include/event2/bufferevent_struct.h"
"${LIBRARY_DIR}/include/event2/buffer_compat.h"
"${LIBRARY_DIR}/include/event2/dns.h"
"${LIBRARY_DIR}/include/event2/dns_compat.h"
"${LIBRARY_DIR}/include/event2/dns_struct.h"
"${LIBRARY_DIR}/include/event2/event.h"
"${LIBRARY_DIR}/include/event2/event_compat.h"
"${LIBRARY_DIR}/include/event2/event_struct.h"
"${LIBRARY_DIR}/include/event2/http.h"
"${LIBRARY_DIR}/include/event2/http_compat.h"
"${LIBRARY_DIR}/include/event2/http_struct.h"
"${LIBRARY_DIR}/include/event2/keyvalq_struct.h"
"${LIBRARY_DIR}/include/event2/listener.h"
"${LIBRARY_DIR}/include/event2/rpc.h"
"${LIBRARY_DIR}/include/event2/rpc_compat.h"
"${LIBRARY_DIR}/include/event2/rpc_struct.h"
"${LIBRARY_DIR}/include/event2/tag.h"
"${LIBRARY_DIR}/include/event2/tag_compat.h"
"${LIBRARY_DIR}/include/event2/thread.h"
"${LIBRARY_DIR}/include/event2/util.h"
"${LIBRARY_DIR}/include/event2/visibility.h"
"${LIBRARY_DIR}/buffer.c"
"${LIBRARY_DIR}/bufferevent.c"
"${LIBRARY_DIR}/bufferevent_filter.c"
"${LIBRARY_DIR}/bufferevent_pair.c"
"${LIBRARY_DIR}/bufferevent_ratelim.c"
"${LIBRARY_DIR}/bufferevent_sock.c"
"${LIBRARY_DIR}/event.c"
"${LIBRARY_DIR}/evmap.c"
"${LIBRARY_DIR}/evthread.c"
"${LIBRARY_DIR}/evutil.c"
"${LIBRARY_DIR}/evutil_rand.c"
"${LIBRARY_DIR}/evutil_time.c"
"${LIBRARY_DIR}/listener.c"
"${LIBRARY_DIR}/log.c"
"${LIBRARY_DIR}/signal.c"
"${LIBRARY_DIR}/strlcpy.c"
"${LIBRARY_DIR}/select.c"
"${LIBRARY_DIR}/poll.c"
"${LIBRARY_DIR}/epoll.c"
"${LIBRARY_DIR}/bufferevent_openssl.c"
"${LIBRARY_DIR}/include/event2/bufferevent_ssl.h"
)

add_library (libevent ${SRCS})

target_include_directories(libevent SYSTEM BEFORE PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_include_directories(libevent SYSTEM BEFORE PUBLIC $<BUILD_INTERFACE:${LIBRARY_DIR}/include>)
