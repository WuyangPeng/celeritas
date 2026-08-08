// 创建时间：2026-08-05
// 修改时间：2026-08-05
// 审核时间：2026-08-05

#include "feedback_admin_list_result.h"

celeritas::feedback_admin_list_result::feedback_admin_list_result(const int64_t total,
                                                                  container_type feedbacks,
                                                                  const int64_t bug_count,
                                                                  const int64_t suggestion_count,
                                                                  const int64_t report_count)
    : total_{ total },
      feedbacks_{ std::move(feedbacks) },
      bug_count_{ bug_count },
      suggestion_count_{ suggestion_count },
      report_count_{ report_count }
{
}

int64_t celeritas::feedback_admin_list_result::get_total() const noexcept
{
    return total_;
}

void celeritas::feedback_admin_list_result::set_total(const int64_t total) noexcept
{
    total_ = total;
}

const celeritas::feedback_admin_list_result::container_type& celeritas::feedback_admin_list_result::get_feedbacks() const noexcept
{
    return feedbacks_;
}

celeritas::feedback_admin_list_result::container_type& celeritas::feedback_admin_list_result::get_feedbacks() noexcept
{
    return feedbacks_;
}

void celeritas::feedback_admin_list_result::set_feedbacks(container_type feedbacks)
{
    feedbacks_ = std::move(feedbacks);
}

int64_t celeritas::feedback_admin_list_result::get_bug_count() const noexcept
{
    return bug_count_;
}

void celeritas::feedback_admin_list_result::set_bug_count(const int64_t bug_count) noexcept
{
    bug_count_ = bug_count;
}

int64_t celeritas::feedback_admin_list_result::get_suggestion_count() const noexcept
{
    return suggestion_count_;
}

void celeritas::feedback_admin_list_result::set_suggestion_count(const int64_t suggestion_count) noexcept
{
    suggestion_count_ = suggestion_count;
}

int64_t celeritas::feedback_admin_list_result::get_report_count() const noexcept
{
    return report_count_;
}

void celeritas::feedback_admin_list_result::set_report_count(const int64_t report_count) noexcept
{
    report_count_ = report_count;
}

