#include "game_error_type.h"

#include <map>

std::string celeritas::get_game_error_description(const game_error_type game_error)
{
    using game_error_container_type = std::map<game_error_type, std::string>;

    static game_error_container_type container{ { game_error_type::unknown, "unknown error." },
                                                { game_error_type::success, "success." },

                                                { game_error_type::invalid_parameter, "invalid parameter" },
                                                { game_error_type::timestamp_expired, "timestamp expired" },
                                                { game_error_type::sign_error, "sign error" },
                                                { game_error_type::sent_too_frequently, "sent too frequently" },
                                                { game_error_type::code_expired, "code expired" },
                                                { game_error_type::code_error, "code error" },
                                                { game_error_type::token_error, "token error" },
                                                { game_error_type::password_error, "password error" },
                                                { game_error_type::sdk_error, "sdk error" },
                                                { game_error_type::server_error, "server error" },
                                                { game_error_type::server_launch_error, "server launch error" },

                                                { game_error_type::redis_error, "redis error" },
                                                { game_error_type::mysql_error, "mysql error" },

                                                { game_error_type::account_error, "account error" },
                                                { game_error_type::no_guest_account, "no guest account" },
                                                { game_error_type::account_bound, "account bound" },
                                                { game_error_type::create_account_error, "create account error" },
                                                { game_error_type::create_user_error, "create user error" },

                                                { game_error_type::duplicate_name, "duplicate name error" },
                                                { game_error_type::insufficient_number_of_name_changes, "insufficient number of name changes" },
                                                { game_error_type::insufficient_item_of_name_change, "insufficient item of name change" },
                                                { game_error_type::name_not_change, "name not change" },

                                                { game_error_type::debug_error, "debug error" },
                                                { game_error_type::item_id_error, "item id error" },
                                                { game_error_type::item_count_error, "item count error" },

                                                { game_error_type::item_selected_error, "item selected error" },
                                                { game_error_type::item_not_exist, "item not exist" },

                                                { game_error_type::max_develop, "max develop" },
                                                { game_error_type::non_resettable, "non resettable" },
                                                { game_error_type::develop_not_exist, "develop not exist" },
                                                { game_error_type::develop_item_not_insufficient, "develop item not insufficient" },

                                                { game_error_type::mail_not_exist, "mail not exist" },
                                                { game_error_type::mail_attachment_collected, "mail attachment collected" },
                                                { game_error_type::mail_no_attachment, "mail no attachment" },
                                                { game_error_type::mail_not_read, "mail not read" },
                                                { game_error_type::mail_attachment_not_collected, "mail attachment not collected" }

    };

    if (const auto iter = container.find(game_error);
        iter != container.cend())
    {
        return iter->second;
    }

    return "error undefined.";
}