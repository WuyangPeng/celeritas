#pragma once

#include <string>

namespace celeritas
{
    enum class game_error_type
    {
        unknown = 0,
        success = 1,

        // 通用错误
        invalid_parameter = 100,
        timestamp_expired = 101,
        sign_error = 102,
        sent_too_frequently = 103,
        code_expired = 104,
        code_error = 105,
        token_error = 106,
        password_error = 107,
        sdk_error = 108,
        server_error = 109,
        server_launch_error = 110,

        // 数据库错误
        redis_error = 1000,
        mysql_error = 1001,
        mongo_error = 1002,

        // 认证服务器错误 10000 - 19999

        // auth错误
        account_error = 10000,
        no_guest_account = 10001,
        account_bound = 10002,
        create_account_error = 10003,
        create_user_error = 10004,

        // 玩家服务器错误 60000 - 69999

        // 角色模块
        duplicate_name = 60100,
        insufficient_number_of_name_changes = 60101,
        insufficient_item_of_name_change = 60102,
        name_not_change = 60103,

        // 调试模块
        debug_error = 60300,
        item_id_error = 60301,
        item_count_error = 60302,

        // 物品模块
        item_selected_error = 60600,
        item_not_exist = 60601,

        // 养成模块
        max_develop = 60700,
        non_resettable = 60701,
        develop_not_exist = 60702,
    };

    [[nodiscard]] std::string get_game_error_description(game_error_type game_error);
}