#pragma once

#include "common/buffer/buffer_pool_data.h"
#include "network/core/mock/mock_session_base.h"

#include <boost/asio/io_context.hpp>

namespace celeritas
{
    class network_message_callback_fixture
    {
    public:
        using class_type = network_message_callback_fixture;
        using io_context_type = boost::asio::io_context;
        using mock_session_base_shared_ptr = std::shared_ptr<mock_session_base>;

        network_message_callback_fixture();

        [[nodiscard]] mock_session_base_shared_ptr get_session();

        [[nodiscard]] buffer_pool_data move_to_buffer_pool_data();

    private:
        io_context_type io_context_;
        buffer_pool_data buffer_data_;
        mock_session_base_shared_ptr session_;
    };
}