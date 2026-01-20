#include "mock_config_table.h"

celeritas::mock_config_table::mock_config_table()
    : name_{ "mock_table" }, load_all_called_{ false }, load_one_called_{ false }
{
}

std::string_view celeritas::mock_config_table::get_name() const
{
    return name_;
}

celeritas::config_table_base::void_awaitable_type celeritas::mock_config_table::load_all(const database_pool_shared_ptr& pool)
{
    load_all_called_ = true;
    co_return;
}

celeritas::config_table_base::void_awaitable_type celeritas::mock_config_table::load_one(const database_pool_shared_ptr& pool, int64_t id)
{
    load_one_called_ = true;
    co_return;
}

bool celeritas::mock_config_table::was_load_all_called() const
{
    return load_all_called_;
}

bool celeritas::mock_config_table::was_load_one_called() const
{
    return load_one_called_;
}

void celeritas::mock_config_table::clear()
{
}
