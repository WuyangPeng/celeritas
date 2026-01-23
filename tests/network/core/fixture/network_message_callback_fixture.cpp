#include "network_message_callback_fixture.h"
#include "config/basic/server_network_type.h"
#include "network/core/mock/mock_network_message_callback.h"

celeritas::network_message_callback_fixture::network_message_callback_fixture()
    : io_context_{},
      buffer_data_{ 1024 },
      session_(std::make_shared<mock_session_base>(io_context_.get_executor(),
                                                   server_network_type::tcp,
                                                   1,
                                                   session_callback{ session_callback::listener_accept_weak_ptr{}, std::make_shared<mock_network_message_callback>() }))
{
}

celeritas::network_message_callback_fixture::mock_session_base_shared_ptr celeritas::network_message_callback_fixture::get_session()
{
    return session_;
}

celeritas::buffer_pool_data celeritas::network_message_callback_fixture::move_to_buffer_pool_data()
{
    return std::move(buffer_data_);
}