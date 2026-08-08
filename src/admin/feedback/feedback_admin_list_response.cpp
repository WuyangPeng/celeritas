// 创建时间：2026-08-05
// 修改时间：2026-08-07
// 审核时间：2026-08-07

#include "feedback_admin_list_response.h"
#include "common/core/celeritas_error.h"
#include "common/core/enum_cast.h"

#include <boost/json.hpp>

celeritas::feedback_admin_list_response::feedback_admin_list_response(const game_error_type code)
    : bass_type{ code }, total_{ 0 }, feedback_{}
{
}

celeritas::feedback_admin_list_response::feedback_admin_list_response(const game_error_type code, std::string message)
    : bass_type{ code, std::move(message) }, total_{ 0 }, feedback_{}
{
}

celeritas::feedback_admin_list_response::feedback_admin_list_response(const game_error_type code,
                                                                      std::string message,
                                                                      const int64_t total,
                                                                      container_type feedback,
                                                                      const int64_t bug_count,
                                                                      const int64_t suggestion_count,
                                                                      const int64_t report_count)
    : bass_type{ code, std::move(message) },
      total_{ total },
      feedback_{ std::move(feedback) },
      bug_count_{ bug_count },
      suggestion_count_{ suggestion_count },
      report_count_{ report_count }
{
}

celeritas::feedback_admin_list_response::feedback_admin_list_response(bass_type http_response,
                                                                      const int64_t total,
                                                                      container_type feedback,
                                                                      const int64_t bug_count,
                                                                      const int64_t suggestion_count,
                                                                      const int64_t report_count)
    : bass_type{ std::move(http_response) },
      total_{ total },
      feedback_{ std::move(feedback) },
      bug_count_{ bug_count },
      suggestion_count_{ suggestion_count },
      report_count_{ report_count }
{
}

celeritas::feedback_admin_list_response::feedback_admin_list_response(bass_type http_response)
    : bass_type{ std::move(http_response) }, total_{ 0 }, feedback_{}, bug_count_{ 0 }, suggestion_count_{ 0 }, report_count_{ 0 }
{
}

int64_t celeritas::feedback_admin_list_response::get_total() const noexcept
{
    return total_;
}

void celeritas::feedback_admin_list_response::set_total(const int64_t total)
{
    total_ = total;
}

const celeritas::feedback_admin_list_response::container_type& celeritas::feedback_admin_list_response::get_feedback() const noexcept
{
    return feedback_;
}

void celeritas::feedback_admin_list_response::set_feedback(container_type feedback)
{
    feedback_ = std::move(feedback);
}

int64_t celeritas::feedback_admin_list_response::get_bug_count() const noexcept
{
    return bug_count_;
}

void celeritas::feedback_admin_list_response::set_bug_count(const int64_t bug_count) noexcept
{
    bug_count_ = bug_count;
}

int64_t celeritas::feedback_admin_list_response::get_suggestion_count() const noexcept
{
    return suggestion_count_;
}

void celeritas::feedback_admin_list_response::set_suggestion_count(const int64_t suggestion_count) noexcept
{
    suggestion_count_ = suggestion_count;
}

int64_t celeritas::feedback_admin_list_response::get_report_count() const noexcept
{
    return report_count_;
}

void celeritas::feedback_admin_list_response::set_report_count(const int64_t report_count) noexcept
{
    report_count_ = report_count;
}

std::string celeritas::feedback_admin_list_response::to_json_string() const
{
    return boost::json::serialize(boost::json::value_from(*this));
}

celeritas::feedback_admin_list_response celeritas::feedback_admin_list_response::from_json_string(const std::string& json_string)
{
    try
    {
        return do_from_json_string(json_string);
    }
    catch (const std::exception& e)
    {
        throw celeritas_error{ "json deserialization failed: {}", e.what() };
    }
}

celeritas::feedback_admin_list_response celeritas::feedback_admin_list_response::tag_invoke(const json_value& value)
{
    auto http_response = bass_type::tag_invoke(value);

    const auto& object = value.as_object();
    const auto total = boost::json::value_to<int64_t>(object.at(total_description));
    auto feedback = boost::json::value_to<container_type>(object.at(feedback_description));

    const auto bug_count = boost::json::value_to<int64_t>(object.at(bug_count_description));
    const auto suggestion_count = boost::json::value_to<int64_t>(object.at(suggestion_count_description));
    const auto report_count = boost::json::value_to<int64_t>(object.at(report_count_description));

    return feedback_admin_list_response{ std::move(http_response), total, std::move(feedback), bug_count, suggestion_count, report_count };
}

celeritas::feedback_admin_list_response celeritas::feedback_admin_list_response::do_from_json_string(const std::string& json_string)
{
    const auto value = boost::json::parse(json_string);
    return boost::json::value_to<feedback_admin_list_response>(value);
}

celeritas::feedback_admin_list_response celeritas::tag_invoke(feedback_admin_list_response_tag, const http_response::json_value& value)
{
    try
    {
        return feedback_admin_list_response::tag_invoke(value);
    }
    catch (const std::out_of_range& error)
    {
        throw celeritas_error{ "json deserialization failed: missing 'code' or 'message' key. {}", error.what() };
    }
    catch (const boost::system::system_error& error)
    {
        throw celeritas_error{ "json deserialization failed: invalid value type for key. {}", error.what() };
    }
}

void celeritas::tag_invoke(boost::json::value_from_tag, feedback_admin_list_response::json_value& value, const feedback_admin_list_response& response)
{
    value = {
        { feedback_admin_list_response::code_description, enum_cast_underlying(response.get_code()) },
        { feedback_admin_list_response::message_description, response.get_message() },
        { feedback_admin_list_response::total_description, response.get_total() },
        { feedback_admin_list_response::bug_count_description, response.get_bug_count() },
        { feedback_admin_list_response::suggestion_count_description, response.get_suggestion_count() },
        { feedback_admin_list_response::report_count_description, response.get_report_count() },
        { feedback_admin_list_response::feedback_description, boost::json::value_from(response.get_feedback()) }
    };
}
