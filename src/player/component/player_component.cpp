#include "player_component.h"
#include "database/database_pool_manager.h"

celeritas::player_component::player_component(const player_component_type player_component_type, player_state* player_state) noexcept
    : player_component_{ player_component_type }, player_state_{ player_state }, mock_database_pool_{}
{
}

celeritas::player_component_type celeritas::player_component::get_player_component_type() const noexcept
{
    return player_component_;
}

celeritas::player_component::void_awaitable_type celeritas::player_component::on_load_db()
{
    co_return;
}

celeritas::player_component::void_awaitable_type celeritas::player_component::on_db_analysis()
{
    co_return;
}

celeritas::player_component::void_awaitable_type celeritas::player_component::on_dependencies_ready()
{
    co_return;
}

celeritas::player_component::void_awaitable_type celeritas::player_component::send_initial_sync()
{
    co_return;
}

celeritas::player_component::void_awaitable_type celeritas::player_component::on_login()
{
    co_return;
}

celeritas::player_component::void_awaitable_type celeritas::player_component::on_logout()
{
    co_return;
}

celeritas::player_component::void_awaitable_type celeritas::player_component::save_db()
{
    co_return;
}

celeritas::player_component::void_awaitable_type celeritas::player_component::time_callback(const player_time_refresh_key& player_time_refresh_key, bool is_login)
{
    co_return;
}

celeritas::player_state* celeritas::player_component::get_player_state()
{
    return player_state_;
}

celeritas::player_component::database_pool_shared_ptr celeritas::player_component::get_mysql_player_database_pool()
{
    return mock_database_pool_ ? *mock_database_pool_ : database_pool_manager::get_instance().get_pool(mysql_player_db_name.data());
}

celeritas::player_component::database_pool_shared_ptr celeritas::player_component::get_mongo_player_database_pool()
{
    return mock_database_pool_ ? *mock_database_pool_ : database_pool_manager::get_instance().get_pool(mongo_player_db_name.data());
}

celeritas::player_component::database_pool_shared_ptr celeritas::player_component::get_mongo_auth_database_pool()
{
    return mock_database_pool_ ? *mock_database_pool_ : database_pool_manager::get_instance().get_pool(mongo_auth_db_name.data());
}

void celeritas::player_component::set_mock_database_pool(const database_pool_shared_ptr& database_pool)
{
    mock_database_pool_ = database_pool;
}

bool celeritas::player_component::is_modify() const
{
    return true;
}

