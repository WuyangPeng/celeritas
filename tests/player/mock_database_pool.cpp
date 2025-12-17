#include "mock_database_pool.h"
#include "common/time_helper.h"
#include "config/database_type.h"
#include "database/database_change_type.h"
#include "database/database_entity_change.h"
#include "database/document/player_time_refresh.h"
#include "database/generated/mongo/player/user_time_refresh.h"
#include "player/component/player_component_type.h"
#include "player/time/time_refresh_type.h"

celeritas::database_pool_base::bool_awaitable_type celeritas::mock_database_pool::execute_changes(const database_entity_change_const_shared_ptr& database, int expiration_time)
{
    ++execute_changes_call_count_;
    co_return true;
}

celeritas::database_pool_base::void_awaitable_type celeritas::mock_database_pool::async_initialize()
{
    co_return;
}

void celeritas::mock_database_pool::cleanup_database_by_duration()
{
}

celeritas::database_pool_base::bool_awaitable_type celeritas::mock_database_pool::is_health()
{
    co_return true;
}

celeritas::database_pool_base::database_entity_change_awaitable_type celeritas::mock_database_pool::select_one(const database_entity_change_const_shared_ptr& database, const database_field_container& field_name_container)
{
    if (database->get_database_name() == user_time_refresh::database_name)
    {
        const basis_database user_id{ "_id", int64_t{ 11111 } };

        player_time_refresh refresh_data{};
        refresh_data.set_time_refresh_type(time_refresh_type::weekly);
        refresh_data.set_parameter(123);
        refresh_data.set_time_id(456);
        refresh_data.add_component(player_component_type::mock);
        refresh_data.set_last_refresh_time(time_helper::get_current_milliseconds());
        basis_database::string_array result{ refresh_data.to_json_string() };
        const basis_database player_time{ user_time_refresh::player_time_refresh_describe, database_data_type::document_array_type, result };

        database_entity_change database_entity_change{ database_type::mongo,
                                                       user_time_refresh::database_name,
                                                       database_change_type::update_type,
                                                       std::make_shared<basis_database_container>(basis_database_container::object_container{ user_id }) };
        database_entity_change.modify(user_id);
        database_entity_change.modify(player_time);

        co_return database_entity_change;
    }

    co_return std::nullopt;
}

celeritas::database_pool_base::result_container_awaitable_type celeritas::mock_database_pool::select_all(const database_entity_change_const_shared_ptr& database, const database_field_container& field_name_container)
{
    co_return result_container{};
}

int celeritas::mock_database_pool::get_execute_changes_call_count() const
{
    return execute_changes_call_count_;
}