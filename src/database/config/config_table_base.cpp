#include "config_table_base.h"

celeritas::config_table_base::void_awaitable_type celeritas::config_table_base::load(const database_pool_shared_ptr& pool, const int64_t id)
{
    if (id == 0)
    {
        co_await load_all(pool);
    }
    else
    {
        co_await load_one(pool, id);
    }
}