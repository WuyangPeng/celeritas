#include "item_flow_log_param.h"

celeritas::item_flow_log_param::item_flow_log_param(std::string action,
                                                    const int64_t item_id,
                                                    const int32_t template_id,
                                                    const int64_t count_change,
                                                    const int64_t before_count,
                                                    const int64_t after_count,
                                                    std::string source_id)
    : action_{ std::move(action) },
      item_id_{ item_id },
      template_id_{ template_id },
      count_change_{ count_change },
      before_count_{ before_count },
      after_count_{ after_count },
      source_id_{ std::move(source_id) }
{
}

const std::string& celeritas::item_flow_log_param::get_action() const noexcept
{
    return action_;
}

int64_t celeritas::item_flow_log_param::get_item_id() const noexcept
{
    return item_id_;
}

int32_t celeritas::item_flow_log_param::get_template_id() const noexcept
{
    return template_id_;
}

int64_t celeritas::item_flow_log_param::get_count_change() const noexcept
{
    return count_change_;
}

int64_t celeritas::item_flow_log_param::get_before_count() const noexcept
{
    return before_count_;
}

int64_t celeritas::item_flow_log_param::get_after_count() const noexcept
{
    return after_count_;
}

const std::string& celeritas::item_flow_log_param::get_source_id() const noexcept
{
    return source_id_;
}
