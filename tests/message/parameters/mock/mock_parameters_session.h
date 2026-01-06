#pragma once

#include "common/framework/session.h"
#include "message/basic/header.h"

namespace celeritas
{
    class mock_parameters_session final : public session
    {
    public:
        using class_type = mock_parameters_session;
        using base_type = session;
        using io_context_type = boost::asio::io_context;

        mock_parameters_session();

        explicit mock_parameters_session(io_context_type& io_context);

        void stop() override;

        void write(const header& header, const protobuf_message_type& response) override;

        [[nodiscard]] void_awaitable_type write_immediately(const std::string& response) override;

        [[nodiscard]] int64_t get_session_id() const noexcept override;

        void remove_session() override;

        [[nodiscard]] server_network_type get_server_network_type() const override;

        void set_instance_id(const std::string& instance_id) override;

        [[nodiscard]] std::string get_instance_id() const override;

        [[nodiscard]] any_io_executor get_any_io_executor() override;

        [[nodiscard]] bool is_write_immediately_call() const;

        [[nodiscard]] bool is_remove_session_call() const;

        [[nodiscard]] int get_write_with_header_count() const;

        [[nodiscard]] header get_last_header() const;

    private:
        int write_with_header_count = 0;
        header last_header;
        bool write_immediately_call_ = false;
        bool remove_session_call_ = false;
        std::string instance_id_;
        boost::asio::any_io_executor executor_;
    };
}
