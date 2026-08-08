// 创建时间：2026-07-30
// 修改时间：2026-07-30
// 审核时间：2026-08-07

#include "admin_list.h"
#include "admin_list_item.h"
#include "admin_list_response.h"
#include "common/core/enum_cast.h"
#include "database/basic/basis_database_container.h"
#include "database/basic/database_select_options.h"
#include "database/database_constant.h"
#include "database/generated/mysql/admin/admin_account.h"
#include "database/generated/redis/admin/admin_session_token.h"
#include "database/pool/database_pool_manager.h"
#include "detail/admin_list_parameter.h"

celeritas::admin_list::admin_list(http_handle_parameter_shared_ptr handle_parameter)
    : base_type{ std::move(handle_parameter) }
{
}

celeritas::admin_list::void_awaitable_type celeritas::admin_list::response()
{
    const admin_list_parameter parameter{ get_http_handle_parameter() };

    if (parameter.is_failure())
    {
        co_return co_await write_immediately(parameter.get_response());
    }

    const auto optional_operator_id = co_await resolve_operator_id(parameter.get_token());
    if (!optional_operator_id)
    {
        co_return co_await write_immediately<admin_list_response>(game_error_type::token_error, "登录已失效，请重新登录");
    }

    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_admin_db_name.data());
    auto result = co_await query_accounts(mysql_pool, parameter.get_app_id(), parameter.get_page(), parameter.get_page_size());

    co_return co_await write_immediately<admin_list_response>(game_error_type::success, "获取成功", result.get_total(), std::move(result.get_admins()));
}

celeritas::http_service_base::void_awaitable_type celeritas::admin_list::send_error_response()
{
    co_return co_await write_immediately<admin_list_response>(game_error_type::unknown);
}

celeritas::admin_list::admin_list_result_awaitable celeritas::admin_list::query_accounts(const database_pool_shared_ptr& mysql_pool,
                                                                                         const int64_t app_id,
                                                                                         const int page,
                                                                                         const int page_size)
{
    const auto key = std::make_shared<basis_database_container>(basis_database{ admin_account::app_id_describe, app_id });

    database_select_options options{};
    options.add_order_by(admin_account::account_id_describe, sort_order::asc);
    options.set_limit(page_size);
    options.set_offset(static_cast<int64_t>(page - 1) * page_size);

    const auto total = co_await mysql_pool->select_count<admin_account>(database_type::mysql, key);
    const auto results = co_await mysql_pool->select_page<admin_account>(database_type::mysql, key, options);

    container_type admin_list{};
    admin_list.reserve(results.size());
    for (const auto& change : results)
    {
        const admin_account account{ change };
        admin_list.emplace_back(std::to_string(account.get_account_id()),
                                account.get_username(),
                                account.get_app_id(),
                                account.get_role(),
                                account.get_nickname(),
                                account.get_avatar(),
                                std::to_string(account.get_creator_account_id()),
                                celeritas::underlying_cast_enum<admin_status_type>(account.get_status()),
                                account.get_create_time(),
                                account.get_update_time(),
                                account.get_last_login_time(),
                                account.get_last_login_ip());
    }

    co_return admin_list_result{ total, std::move(admin_list) };
}
