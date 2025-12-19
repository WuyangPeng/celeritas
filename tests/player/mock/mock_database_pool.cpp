#include "mock_database_pool.h"
#include "common/time_helper.h"
#include "config/database_type.h"
#include "config/game_config/red_dot_type.h"
#include "database/database_change_type.h"
#include "database/database_entity_change.h"
#include "database/document/player_time_refresh.h"
#include "database/document/red_dots.h"
#include "database/document/server_role.h"
#include "database/generated/mongo/auth/user_server_roles.h"
#include "database/generated/mongo/player/user_red_dots.h"
#include "database/generated/mongo/player/user_role.h"
#include "database/generated/mongo/player/user_time_refresh.h"
#include "database/generated/mysql/auth/account_last_login.h"
#include "database/generated/mysql/player/user.h"
#include "database/generated/mysql/auth/account.h"
#include "database/generated/mysql/auth/account_bind.h"
#include "player/component/player_component_type.h"
#include "player/time/time_refresh_type.h"

celeritas::database_pool_base::bool_awaitable_type celeritas::mock_database_pool::execute_changes(const database_entity_change_const_shared_ptr& database, int expiration_time)
{
    ++execute_changes_call_count_;
    co_return execute_changes_result_;
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

celeritas::database_pool_base::optional_database_entity_change_awaitable_type celeritas::mock_database_pool::select_one(const database_entity_change_const_shared_ptr& database, const database_field_container& field_name_container)
{
    ++select_one_call_count_;
    if (!select_one_result_)
    {
        co_return std::nullopt;
    }

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

    if (database->get_database_name() == user::database_name)
    {
        co_return select_mock_user();
    }

    if (database->get_database_name() == account::database_name)
    {
        co_return select_mock_account();
    }

    if (database->get_database_name() == account_bind::database_name)
    {
        co_return select_mock_account_bind();
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

int celeritas::mock_database_pool::get_select_one_call_count() const
{
    return select_one_call_count_;
}

void celeritas::mock_database_pool::set_execute_changes_result(const bool result)
{
    execute_changes_result_ = result;
}

void celeritas::mock_database_pool::set_select_one_result(const bool result)
{
    select_one_result_ = result;
}

celeritas::database_entity_change celeritas::mock_database_pool::select_user_time_refresh()
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

celeritas::database_entity_change celeritas::mock_database_pool::select_user_role()
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

celeritas::database_entity_change celeritas::mock_database_pool::select_user_server_roles()
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

celeritas::database_entity_change celeritas::mock_database_pool::select_account_last_login()
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

celeritas::database_entity_change celeritas::mock_database_pool::select_user_red_dots()
{
    const basis_database user_id{ "_id", int64_t{ 11111 } };
    const basis_database last_check_time{ user_red_dots::last_check_time_describe, time_helper::get_current_milliseconds() };

    red_dots red_dot{};
    red_dot.set_node_id(red_dot_type::null);
    red_dot.set_state(false);
    red_dot.set_last_value(0);
    red_dot.set_update_time(time_helper::get_current_milliseconds());
    basis_database::string_array result{ red_dot.to_json_string() };
    const basis_database red_dots{ user_red_dots::red_dots_describe, database_data_type::document_array_type, result };

    database_entity_change database_entity_change{ database_type::mongo,
                                                   user_red_dots::database_name,
                                                   database_change_type::update_type,
                                                   std::make_shared<basis_database_container>(basis_database_container::object_container{ user_id }) };
    database_entity_change.modify(user_id);
    database_entity_change.modify(last_check_time);
    database_entity_change.modify(red_dots);

    return database_entity_change;
}

celeritas::database_entity_change celeritas::mock_database_pool::select_mock_user()
{
    const basis_database user_id{ user::user_id_describe, int64_t{ 12345 } };
    const basis_database account_id{ user::account_id_describe, int64_t{ 111 } };
    const basis_database game_server_id{ user::game_server_id_describe, std::string{ "test_server" } };
    const basis_database overload_db{ user::overload_db_describe, false };

    database_entity_change database_entity_change{ database_type::mysql,
                                                   user::database_name,
                                                   database_change_type::update_type,
                                                   std::make_shared<basis_database_container>(basis_database_container::object_container{ user_id }) };
    database_entity_change.modify(user_id);
    database_entity_change.modify(account_id);
    database_entity_change.modify(game_server_id);
    database_entity_change.modify(overload_db);

    return database_entity_change;
}

celeritas::database_entity_change celeritas::mock_database_pool::select_mock_account()
{
    const basis_database account_id{ account::account_id_describe, int64_t{ 111 } };
    const basis_database account_name{ account::account_name_describe, std::string{ "account_name" } };
    const basis_database password{ account::password_hash_describe, std::string{ "password" } };
    const basis_database salt{ account::salt_describe, std::string{ "salt" } };
    const basis_database device_id{ account::device_id_describe, std::string{ "device_id" } };
    const basis_database app_id{ account::app_id_describe, int64_t{ 123 } };
    const basis_database create_time{ account::create_time_describe, time_helper::get_current_milliseconds() };
    const basis_database status{ account::status_describe, 0 };

    database_entity_change database_entity_change{ database_type::mysql,
                                                   account::database_name,
                                                   database_change_type::update_type,
                                                   std::make_shared<basis_database_container>(basis_database_container::object_container{ account_id }) };
    database_entity_change.modify(account_id);
    database_entity_change.modify(account_name);
    database_entity_change.modify(password);
    database_entity_change.modify(salt);
    database_entity_change.modify(device_id);
    database_entity_change.modify(app_id);
    database_entity_change.modify(create_time);
    database_entity_change.modify(status);

    return database_entity_change;
}

celeritas::database_entity_change celeritas::mock_database_pool::select_mock_account_bind()
{
    const basis_database account_bind_id{ account_bind::account_bind_id_describe, int64_t{ 123 } };
    const basis_database account_id{ account_bind::account_id_describe, int64_t{ 111 } };
    const basis_database account_type{ account_bind::account_type_describe, 1 };
    const basis_database process_type{ account_bind::process_type_describe, 1 };
    const basis_database app_id{ account_bind::app_id_describe, int64_t{ 111 } };
    const basis_database auth_key{ account_bind::auth_key_describe, std::string{ "auth_key" } };

    database_entity_change database_entity_change{ database_type::mysql,
                                                   account_bind::database_name,
                                                   database_change_type::update_type,
                                                   std::make_shared<basis_database_container>(basis_database_container::object_container{ account_bind_id }) };
    database_entity_change.modify(account_bind_id);
    database_entity_change.modify(account_id);
    database_entity_change.modify(account_type);
    database_entity_change.modify(process_type);
    database_entity_change.modify(app_id);
    database_entity_change.modify(auth_key);

    return database_entity_change;
}
