#include "mock_database_pool.h"
#include "common/time_helper.h"
#include "config/database_type.h"
#include "database/database_change_type.h"
#include "database/database_entity_change.h"
#include "database/document/player_time_refresh.h"
#include "database/document/server_role.h"
#include "database/generated/mongo/auth/user_server_roles.h"
#include "database/generated/mongo/player/user_role.h"
#include "database/generated/mongo/player/user_time_refresh.h"
#include "database/generated/mysql/auth/account_last_login.h"
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
        co_return select_user_time_refresh();
    }

    if (database->get_database_name() == user_role::database_name)
    {
        co_return select_user_role();
    }

    if (database->get_database_name() == user_server_roles::database_name)
    {
        co_return select_user_server_roles();
    }

    if (database->get_database_name() == account_last_login::database_name)
    {
        co_return select_account_last_login();
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

celeritas::database_pool_base::optional_database_entity_change celeritas::mock_database_pool::select_user_time_refresh()
{
    const basis_database user_id{ "_id", int64_t{ 11111 } };

    player_time_refresh refresh_data{};
    refresh_data.set_time_refresh_type(time_refresh_type::weekly);
    refresh_data.set_parameter(123);
    refresh_data.set_time_id(456);
    refresh_data.add_component(player_component_type::finish);
    refresh_data.set_last_refresh_time(time_helper::get_current_milliseconds());
    basis_database::string_array result{ refresh_data.to_json_string() };
    const basis_database player_time{ user_time_refresh::player_time_refresh_describe, database_data_type::document_array_type, result };

    database_entity_change database_entity_change{ database_type::mongo,
                                                   user_time_refresh::database_name,
                                                   database_change_type::update_type,
                                                   std::make_shared<basis_database_container>(basis_database_container::object_container{ user_id }) };
    database_entity_change.modify(user_id);
    database_entity_change.modify(player_time);

    return database_entity_change;
}

celeritas::database_pool_base::optional_database_entity_change celeritas::mock_database_pool::select_user_role()
{
    const basis_database user_id{ "_id", int64_t{ 11111 } };
    const basis_database name{ user_role::name_describe, std::string{ "test_name" } };
    const basis_database device_id{ user_role::device_id_describe, std::string{ "test_device" } };
    const basis_database app_version{ user_role::app_version_describe, std::string{ "1.0.0" } };

    database_entity_change database_entity_change{ database_type::mongo,
                                                   user_role::database_name,
                                                   database_change_type::update_type,
                                                   std::make_shared<basis_database_container>(basis_database_container::object_container{ user_id }) };
    database_entity_change.modify(user_id);
    database_entity_change.modify(name);
    database_entity_change.modify(device_id);
    database_entity_change.modify(app_version);

    return database_entity_change;
}

celeritas::database_pool_base::optional_database_entity_change celeritas::mock_database_pool::select_user_server_roles()
{
    const basis_database user_id{ "_id", int64_t{ 11111 } };
    const basis_database update_time{ user_server_roles::update_time_describe, time_helper::get_current_milliseconds() };

    server_role server_role{};
    server_role.set_role_name("test_name");
    server_role.set_game_server_id("test_game_server_id");
    server_role.set_last_login_time(time_helper::get_current_milliseconds());
    basis_database::string_array result{ server_role.to_json_string() };
    const basis_database servers{ user_server_roles::servers_describe, database_data_type::document_array_type, result };

    database_entity_change database_entity_change{ database_type::mongo,
                                                   user_server_roles::database_name,
                                                   database_change_type::update_type,
                                                   std::make_shared<basis_database_container>(basis_database_container::object_container{ user_id }) };
    database_entity_change.modify(user_id);
    database_entity_change.modify(update_time);
    database_entity_change.modify(servers);

    return database_entity_change;
}

celeritas::database_pool_base::optional_database_entity_change celeritas::mock_database_pool::select_account_last_login()
{
    const basis_database account_id{ account_last_login::account_id_describe, int64_t{ 11111 } };
    const basis_database game_server_id{ account_last_login::game_server_id_describe, std::string{ "test_name" } };
    const basis_database update_time{ account_last_login::update_time_describe, time_helper::get_current_milliseconds() };

    database_entity_change database_entity_change{ database_type::mysql,
                                                   account_last_login::database_name,
                                                   database_change_type::update_type,
                                                   std::make_shared<basis_database_container>(basis_database_container::object_container{ account_id }) };
    database_entity_change.modify(account_id);
    database_entity_change.modify(game_server_id);
    database_entity_change.modify(update_time);

    return database_entity_change;
}