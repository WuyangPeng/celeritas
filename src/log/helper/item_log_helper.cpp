#include "item_log_helper.h"
#include "log/core/log_client.h"
#include "proto/service/log.pb.h"

void celeritas::item_log_helper::add_item_flow_log(const log_param& param, const item_flow_log_param& item_param)
{
    proto::service::service_log log{};

    fill_log_header(log, param);
    fill_item_flow_log(log, item_param);
    log_client::get_instance().add_log(log);
}

void celeritas::item_log_helper::fill_log_header(service_log_type& log, const log_param& param)
{
    auto* header = log.mutable_header();
    header->set_player_id(param.get_player_id());
    header->set_timestamp(param.get_timestamp());
    header->set_ip_address(param.get_ip_address());
}

void celeritas::item_log_helper::fill_item_flow_log(service_log_type& log, const item_flow_log_param& item_param)
{
    auto* item = log.mutable_item();
    auto* item_flow = item->mutable_item_flow();
    item_flow->set_action(item_param.get_action());
    item_flow->set_item_id(item_param.get_item_id());
    item_flow->set_template_id(item_param.get_template_id());
    item_flow->set_count_change(item_param.get_count_change());
    item_flow->set_before_count(item_param.get_before_count());
    item_flow->set_after_count(item_param.get_after_count());
    item_flow->set_source_id(item_param.get_source_id());
}
