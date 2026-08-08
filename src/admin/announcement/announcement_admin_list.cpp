// 创建时间：2026-08-06
// 修改时间：2026-08-06
// 审核时间：2026-08-08

#include "announcement_admin_list.h"
#include "announcement_admin_list_item.h"
#include "announcement_admin_list_response.h"
#include "admin/admin_constant.h"
#include "common/core/enum_cast.h"
#include "database/database_constant.h"
#include "database/basic/basis_database_container.h"
#include "database/generated/mysql/admin/announcement.h"
#include "database/pool/database_pool_manager.h"
#include "detail/announcement_admin_list_parameter.h"

#include <algorithm>
#include <cctype>

celeritas::announcement_admin_list::announcement_admin_list(http_handle_parameter_shared_ptr handle_parameter)
    : base_type{ std::move(handle_parameter) }
{
}

celeritas::announcement_admin_list::void_awaitable_type celeritas::announcement_admin_list::response()
{
    const announcement_admin_list_parameter parameter{ get_http_handle_parameter() };

    if (parameter.is_failure())
    {
        co_return co_await write_immediately(parameter.get_response());
    }

    if (const auto optional_operator_id = co_await resolve_operator_id(parameter.get_token()); !optional_operator_id)
    {
        co_return co_await write_immediately<announcement_admin_list_response>(game_error_type::token_error, "登录已失效，请重新登录");
    }

    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_admin_db_name.data());
    auto result = co_await query_announcements(mysql_pool,
                                               parameter.get_app_id(),
                                               parameter.get_page(),
                                               parameter.get_page_size(),
                                               parameter.get_keyword(),
                                               parameter.get_tag(),
                                               parameter.get_status());

    co_return co_await write_immediately<announcement_admin_list_response>(game_error_type::success,
                                                                           "获取成功",
                                                                           result.get_total(),
                                                                           std::move(result.get_announcements()));
}

celeritas::http_service_base::void_awaitable_type celeritas::announcement_admin_list::send_error_response()
{
    co_return co_await write_immediately<announcement_admin_list_response>(game_error_type::unknown);
}

celeritas::announcement_admin_list::announcement_admin_list_result_awaitable celeritas::announcement_admin_list::query_announcements(const database_pool_shared_ptr& mysql_pool,
                                                                                                                                     const int64_t app_id,
                                                                                                                                     const int page,
                                                                                                                                     const int page_size,
                                                                                                                                     const std::string& keyword,
                                                                                                                                     const optional_int32& tag,
                                                                                                                                     const optional_announcement_status& status)
{
    basis_database_container::object_container conditions;
    conditions.emplace_back(announcement::app_id_describe, app_id);

    if (tag.has_value())
    {
        conditions.emplace_back(announcement::tag_describe, tag.value());
    }

    if (status.has_value())
    {
        conditions.emplace_back(announcement::status_describe, enum_cast_underlying(status.value()));
    }

    const auto key = std::make_shared<basis_database_container>(std::move(conditions));

    database_select_options options{};
    options.add_order_by(announcement::priority_describe, sort_order::desc);
    options.add_order_by(announcement::publish_time_describe, sort_order::desc);
    options.set_limit(page_size);
    options.set_offset(static_cast<int64_t>(page - 1) * page_size);

    if (!keyword.empty())
    {
        options.add_like_condition({ announcement::title_describe, announcement::content_describe }, keyword);
    }

    const auto total = co_await mysql_pool->select_count<announcement>(database_type::mysql, key, options);
    const auto results = co_await mysql_pool->select_page<announcement>(database_type::mysql, key, options);

    container_type announcements;
    announcements.reserve(results.size());
    for (const auto& change : results)
    {
        const announcement item{ change };
        announcements.emplace_back(item.get_id(),
                                   item.get_app_id(),
                                   item.get_title(),
                                   item.get_tag(),
                                   item.get_banner_url(),
                                   item.get_content(),
                                   item.get_priority(),
                                   item.get_publish_time(),
                                   underlying_cast_enum<announcement_status>(item.get_status()),
                                   item.get_server(),
                                   item.get_create_time(),
                                   item.get_update_time());
    }

    co_return announcement_admin_list_result{ total, std::move(announcements) };
}
