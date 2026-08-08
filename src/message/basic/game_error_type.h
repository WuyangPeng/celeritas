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

        // 后台服务器错误 30000 - 39999
        feedback_daily_limit_exceeded = 30000,
        username_already_exists = 30001,
        admin_account_not_found = 30002,
        cannot_delete_super_admin = 30003,
        cannot_delete_self = 30004,
        insufficient_permissions = 30005,
        admin_account_disabled = 30006,
        cannot_disable_super_admin = 30007,
        cannot_update_self_status = 30008,
        announcement_not_found = 30009,

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
        item_already_exist = 60602,
        item_locked = 60603,

        // 养成模块
        max_develop = 60700,
        non_resettable = 60701,
        develop_not_exist = 60702,
        develop_item_not_insufficient = 60703,
        develop_reward_already_claimed = 60704,

        // 邮件模块
        mail_not_exist = 60800,
        mail_attachment_collected = 60801,
        mail_no_attachment = 60802,
        mail_not_read = 60803,
        mail_attachment_not_collected = 60804,

        // 任务模块
        task_not_exist = 60900,
        task_progress_not_enough = 60901,
    };

    [[nodiscard]] std::string get_game_error_description(game_error_type game_error);
}