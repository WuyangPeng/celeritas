#include "gateway_resource_loader.h"
#include "common/logger/logger.h"
#include "detail/gateway_server_fwd.h"
#include "initializer/initializer_fwd.h"
#include "message/header.h"
#include "proto/common/common.pb.h"

#include <boost/polymorphic_pointer_cast.hpp>

celeritas::gateway_resource_loader::gateway_resource_loader(const std::string_view server_type, app_config_shared_ptr app_config)
    : base_type{ server_type, std::move(app_config) }, player_check_timer_{}
{
}

void celeritas::gateway_resource_loader::service_initialize_resource(io_context_type& io_context, const network_message_callback_weak_ptr& network_message_callback)
{
    start_player_check_timer(io_context, network_message_callback);
}

void celeritas::gateway_resource_loader::send_player_check()
{
    proto::celeritas request{};
    auto* server_discover = request.mutable_celeritas_request()->mutable_service()->mutable_registry()->mutable_server_discover();
    server_discover->set_service_name(player_type);

    if (!write(service_registry_type.data(), header{ proto::common::empty_message_header{} }, request))
    {
        LOG_CHANNEL(initializer_channel, warning) << "write server discover error.";
    }
}

void celeritas::gateway_resource_loader::start_player_check_timer(io_context_type& io_context, const network_message_callback_weak_ptr& network_message_callback)
{
    player_check_timer_ = std::make_unique<player_check_timer>(io_context,
                                                               player_check_time,
                                                               boost::polymorphic_pointer_downcast<class_type>(shared_from_this()),
                                                               network_message_callback);

    player_check_timer_->start();
}