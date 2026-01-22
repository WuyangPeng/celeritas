#pragma once

#include "common/framework/session.h"

namespace celeritas
{
    class mock_session : public session
    {
    public:
        using class_type = mock_session;
        using base_type = session;

        [[nodiscard]] int64_t get_session_id() const noexcept override;

        void write(const header& header, const protobuf_message_type& response) override;

        [[nodiscard]] void_awaitable_type write_immediately(const std::string& response) override;

        void remove_session() override;

        [[nodiscard]] server_network_type get_server_network_type() const override;

        void set_instance_id(const std::string& instance_id) override;

        [[nodiscard]] std::string get_instance_id() const override;
    };
}