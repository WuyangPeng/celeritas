#include "item_flow_log_write.h"
#include "common/core/snowflake_generator.h"
#include "common/logging/logger.h"
#include "config/aggregate/app_config.h"
#include "config/basic/database_type.h"
#include "database/database_constant.h"
#include "database/document/item_flow.h"
#include "database/document/log_header.h"
#include "database/generated/mongo/log/item_flow_log.h"
#include "database/pool/database_pool_manager.h"
#include "message/basic/game_error_type.h"
#include "proto/celeritas.pb.h"

celeritas::item_flow_log_write::item_flow_log_write(protobuf_handle_parameter_shared_ptr protobuf_handle_parameter, const proto::service::service_log_request& request)
    : protobuf_handle_parameter_{ std::move(protobuf_handle_parameter) }, request_{ request }
{
}

void celeritas::item_flow_log_write::send_error_message(const game_error_type game_error_type) const
{
    LOG_CHANNEL(log_channel, warning) << "item_flow_log_write error: " << static_cast<int>(game_error_type);
}

celeritas::item_flow_log_write::void_awaitable_type celeritas::item_flow_log_write::response() const
{
    const auto mongo_pool = database_pool_manager::get_instance().get_pool(mongo_log_db_name.data());
    if (!mongo_pool)
    {
        LOG_CHANNEL(log_channel, error) << "Failed to get mongo_log_db pool.";
        co_return;
    }

    const auto app_config = protobuf_handle_parameter_->get_app_config();
    const auto server_config = app_config->get_server_config();

    for (const auto& log : request_.service_log())
    {
        if (log.has_item() && log.item().has_item_flow())
        {
            const auto& header_proto = log.header();
            const auto& flow_proto = log.item().item_flow();

            const auto log_id = snowflake_generator::get_instance().generate(server_config->get_datacenter_id(), server_config->get_worker_id());

            log_header header_doc{ header_proto.player_id(), header_proto.timestamp(), header_proto.ip_address() };
            item_flow flow_doc{ flow_proto.action(), flow_proto.item_id(), flow_proto.template_id(), flow_proto.count_change(), flow_proto.before_count(), flow_proto.after_count(), flow_proto.source_id() };

            auto log_entity = std::make_shared<item_flow_log>(database_type::mongo, log_id);
            log_entity->set_header(header_doc.to_document_type());
            log_entity->set_item_flow(flow_doc.to_document_type());

            if (co_await mongo_pool->execute_changes(log_entity->get_modify()))
            {
                LOG_CHANNEL(log_channel, debug) << "Successfully wrote item flow log to DB. log_id = " << log_id << ", player_id = " << header_proto.player_id();
            }
            else
            {
                LOG_CHANNEL(log_channel, error) << "Failed to write item flow log to DB. log_id = " << log_id;
            }
        }
    }

    co_return;
}
