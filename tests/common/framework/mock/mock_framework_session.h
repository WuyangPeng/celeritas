#pragma once

#include "common/framework/session.h"
#include "message/basic/header.h"

#include <boost/asio/awaitable.hpp>

namespace celeritas
{
    class mock_framework_session final : public session
    {
    public:
        using class_type = mock_framework_session;
        using base_type = session;

        void stop() override;

        void write(const header& header, const protobuf_message_type& response) override;

        void write(const std::string& response) override;

        [[nodiscard]] void_awaitable_type write_immediately(const std::string& response) override;

        [[nodiscard]] int64_t get_session_id() const noexcept override;

        void remove_session() override;

        [[nodiscard]] server_network_type get_server_network_type() const override;

        void set_instance_id(const std::string& instance_id) override;

        [[nodiscard]] std::string get_instance_id() const override;

        [[nodiscard]] any_io_executor get_any_io_executor() override;

    private:
        std::string instance_id_;
    };
}
