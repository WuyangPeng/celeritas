// 创建时间：2026-08-05
// 修改时间：2026-08-07
// 审核时间：2026-08-07

#pragma once

#include "feedback_admin_list_item.h"
#include "message/basic/http_response.h"

#include <vector>

namespace celeritas
{
    class feedback_admin_list_response final : public http_response
    {
    public:
        using class_type = feedback_admin_list_response;
        using bass_type = http_response;
        using container_type = std::vector<feedback_admin_list_item>;

        feedback_admin_list_response() noexcept = default;

        explicit feedback_admin_list_response(game_error_type code);

        feedback_admin_list_response(game_error_type code, std::string message);

        feedback_admin_list_response(game_error_type code, std::string message, int64_t total, container_type feedback, int64_t bug_count, int64_t suggestion_count, int64_t report_count);

        feedback_admin_list_response(bass_type http_response, int64_t total, container_type feedback, int64_t bug_count, int64_t suggestion_count, int64_t report_count);

        explicit feedback_admin_list_response(bass_type http_response);

        [[nodiscard]] int64_t get_total() const noexcept;

        void set_total(int64_t total);

        [[nodiscard]] const container_type& get_feedback() const noexcept;

        void set_feedback(container_type feedback);

        [[nodiscard]] int64_t get_bug_count() const noexcept;

        void set_bug_count(int64_t bug_count) noexcept;

        [[nodiscard]] int64_t get_suggestion_count() const noexcept;

        void set_suggestion_count(int64_t suggestion_count) noexcept;

        [[nodiscard]] int64_t get_report_count() const noexcept;

        void set_report_count(int64_t report_count) noexcept;

        [[nodiscard]] std::string to_json_string() const override;

        [[nodiscard]] static feedback_admin_list_response from_json_string(const std::string& json_string);

        [[nodiscard]] static feedback_admin_list_response tag_invoke(const json_value& value);

        static constexpr std::string_view total_description = "total";
        static constexpr std::string_view feedback_description = "feedback";
        static constexpr std::string_view bug_count_description = "bug_count";
        static constexpr std::string_view suggestion_count_description = "suggestion_count";
        static constexpr std::string_view report_count_description = "report_count";

    private:
        [[nodiscard]] static feedback_admin_list_response do_from_json_string(const std::string& json_string);

        int64_t total_ = 0;
        container_type feedback_;
        int64_t bug_count_ = 0;
        int64_t suggestion_count_ = 0;
        int64_t report_count_ = 0;
    };

    using feedback_admin_list_response_tag = boost::json::value_to_tag<feedback_admin_list_response>;

    [[nodiscard]] feedback_admin_list_response tag_invoke(feedback_admin_list_response_tag, const http_response::json_value& value);

    void tag_invoke(boost::json::value_from_tag tag, feedback_admin_list_response::json_value& value, const feedback_admin_list_response& response);
}
