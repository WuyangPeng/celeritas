#include "time_refresh_table.h"
#include "config/basic/database_type.h"
#include "database/database_constant.h"
#include "database/pool/database_pool_base.h"

celeritas::time_refresh_table::time_refresh_table()
    : name_{ time_refresh_db_name }, container_{}, shared_mutex_{}
{
}

const std::string& celeritas::time_refresh_table::get_name() const
{
    return name_;
}

celeritas::config_table_base::void_awaitable_type celeritas::time_refresh_table::load_all(const database_pool_shared_ptr& pool)
{
    const auto result = co_await pool->select_all(time_refresh::get_select(database_type::mysql), time_refresh::get_database_field_container());

    time_refresh_container_type container{};
    for (const auto& row : result)
    {
        const time_refresh item{ row };
        container.emplace(item.get_id(), std::make_shared<time_refresh>(item));
    }

    std::lock_guard lock{ shared_mutex_ };
    container_ = std::move(container);
}

celeritas::config_table_base::void_awaitable_type celeritas::time_refresh_table::load_one(const database_pool_shared_ptr& pool, const int64_t id)
{
    if (id == 0)
    {
        co_return co_await load_all(pool);
    }

    if (const auto optional_item = co_await pool->select_one(time_refresh::get_select(database_type::mysql, id), time_refresh::get_database_field_container()))
    {
        const time_refresh item{ *optional_item };

        std::lock_guard lock{ shared_mutex_ };
        container_.insert_or_assign(item.get_id(), std::make_shared<time_refresh>(item));
    }
}

celeritas::time_refresh_table::optional_const_time_refresh_shared_ptr celeritas::time_refresh_table::get_time_refresh(const int64_t id)
{
    std::shared_lock lock{ shared_mutex_ };

    if (const auto iter = container_.find(id);
        iter != container_.cend())
    {
        return iter->second;
    }

    return std::nullopt;
}
