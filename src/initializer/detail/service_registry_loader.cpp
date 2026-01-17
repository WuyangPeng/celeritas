#include "service_registry_loader.h"
#include "config/local/service_registry_config.h"
#include "network/client/tcp_client.h"

celeritas::service_registry_loader::tcp_client_shared_ptr celeritas::service_registry_loader::loader_service_registry(const any_io_executor& any_io_executor,
                                                                                                                      const service_registry_config& service_registry_config,
                                                                                                                      const network_message_callback_weak_ptr& network_message_callback,
                                                                                                                      const std::string& game_server_id,
                                                                                                                      const std::string& server_type)
{
    const auto client = std::make_shared<tcp_client>(any_io_executor, network_message_callback, game_server_id, service_registry_config.get_name(), service_registry_config.get_host(), service_registry_config.get_port(), server_type);

    boost::asio::co_spawn(any_io_executor,
                          client->connect(),
                          boost::asio::detached);

    return client;
}