// 创建时间：2026-08-05
// 修改时间：2026-08-05
// 审核时间：2026-08-05

#include "feedback_admin_list.h"
#include "feedback_admin_list_item.h"
#include "feedback_admin_list_response.h"
#include "database/basic/basis_database_container.h"
#include "feedback_type.h"
#include "common/core/enum_cast.h"
#include "database/basic/database_select_options.h"
#include "database/database_constant.h"
#include "database/generated/mysql/admin/feedback.h"
#include "database/pool/database_pool_manager.h"
#include "detail/feedback_admin_list_parameter.h"

celeritas::feedback_admin_list::feedback_admin_list(http_handle_parameter_shared_ptr handle_parameter)
    : base_type{ std::move(handle_parameter) }
{
}

celeritas::feedback_admin_list::void_awaitable_type celeritas::feedback_admin_list::response()
{
    const feedback_admin_list_parameter parameter{ get_http_handle_parameter() };

    if (parameter.is_failure())
    {
        co_return co_await write_immediately(parameter.get_response());
    }

    const auto optional_operator_id = co_await resolve_operator_id(parameter.get_token());
    if (!optional_operator_id)
    {
        co_return co_await write_immediately<feedback_admin_list_response>(game_error_type::token_error, "登录已失效，请重新登录");
    }

    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_admin_db_name.data());
    auto result = co_await query_feedbacks(mysql_pool, parameter.get_app_id(), parameter.get_page(), parameter.get_page_size());

    co_return co_await write_immediately<feedback_admin_list_response>(game_error_type::success,
                                                                       "获取成功",
                                                                       result.get_total(),
                                                                       std::move(result.get_feedbacks()),
                                                                       result.get_bug_count(),
                                                                       result.get_suggestion_count(),
                                                                       result.get_report_count());
}

celeritas::http_service_base::void_awaitable_type celeritas::feedback_admin_list::send_error_response()
{
    co_return co_await write_immediately<feedback_admin_list_response>(game_error_type::unknown);
}

celeritas::feedback_admin_list::feedback_admin_list_result_awaitable celeritas::feedback_admin_list::query_feedbacks(const database_pool_shared_ptr& mysql_pool,
                                                                                                                     const int64_t app_id,
                                                                                                                     const int page,
                                                                                                                     const int page_size)
{
    const auto key = std::make_shared<basis_database_container>(basis_database{ feedback::app_id_describe, app_id });

    const auto total = co_await mysql_pool->select_count<feedback>(database_type::mysql, key);

    const auto bug_key = std::make_shared<basis_database_container>(basis_database_container::object_container{ basis_database{ feedback::app_id_describe, app_id },
                                                                                                                basis_database{ feedback::type_describe, enum_cast_underlying(feedback_type::bug) } });
    const auto suggestion_key = std::make_shared<basis_database_container>(basis_database_container::object_container{ basis_database{ feedback::app_id_describe, app_id },
                                                                                                                       basis_database{ feedback::type_describe, enum_cast_underlying(feedback_type::suggestion) } });
    const auto report_key = std::make_shared<basis_database_container>(basis_database_container::object_container{ basis_database{ feedback::app_id_describe, app_id },
                                                                                                                   basis_database{ feedback::type_describe, enum_cast_underlying(feedback_type::report) } });

    const auto bug_count = co_await mysql_pool->select_count<feedback>(database_type::mysql, bug_key);
    const auto suggestion_count = co_await mysql_pool->select_count<feedback>(database_type::mysql, suggestion_key);
    const auto report_count = co_await mysql_pool->select_count<feedback>(database_type::mysql, report_key);

    database_select_options options{};
    options.add_order_by(feedback::create_time_describe, sort_order::desc);
    options.set_limit(page_size);
    options.set_offset(static_cast<int64_t>(page - 1) * page_size);

    const auto results = co_await mysql_pool->select_page<feedback>(database_type::mysql, key, options);

    container_type feedbacks{};
    feedbacks.reserve(results.size());
    for (const auto& change : results)
    {
        const feedback item{ change };
        const auto& raw_image = item.get_image_data();
        std::string image(raw_image.begin(), raw_image.end());

        feedbacks.emplace_back(item.get_id(),
                               item.get_app_id(),
                               item.get_type(),
                               item.get_content(),
                               item.get_device_info(),
                               item.get_server(),
                               item.get_player_id(),
                               item.is_is_anonymous(),
                               std::move(image),
                               item.get_create_time());
    }

    co_return feedback_admin_list_result{ total, std::move(feedbacks), bug_count, suggestion_count, report_count };
}
