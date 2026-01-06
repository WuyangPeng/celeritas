#include "create_user.h"
#include "common/core/snowflake_generator.h"
#include "config/aggregate/app_config.h"
#include "config/basic/database_type.h"
#include "database/database_constant.h"
#include "database/pool/database_pool_manager.h"
#include "database/generated/mysql/player/user.h"
#include "message/parameters/protobuf_handle_parameter.h"

celeritas::create_user::create_user(protobuf_handle_parameter_shared_ptr protobuf_handle_parameter, const proto::service::service_login_request& login)
    : protobuf_handle_parameter_{ std::move(protobuf_handle_parameter) }, login_{ login }
{
}

celeritas::create_user::optional_user_awaitable_type celeritas::create_user::save_database() const
{
    const auto server_config = protobuf_handle_parameter_->get_app_config()->get_server_config();
    const auto user_id = snowflake_generator::get_instance().generate(server_config->get_datacenter_id(), server_config->get_worker_id());

    user user{ database_type::mysql, user_id };
    user.set_account_id(login_.account_id());
    user.set_game_server_id(login_.game_server_id());
    user.set_overload_db(true);

    if (const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_player_db_name.data());
        !co_await mysql_pool->execute_changes(user.get_modify()))
    {
        co_return std::nullopt;
    }

    co_return user;
}