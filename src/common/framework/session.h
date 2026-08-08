#pragma once

#include "config/config_fwd.h"
#include "message/message_fwd.h"

#include <boost/asio/awaitable.hpp>
#include <google/protobuf/message.h>

#include <memory>

namespace celeritas
{
    class session : public std::enable_shared_from_this<session>
    {
    public:
        using class_type = session;
        using any_io_executor = boost::asio::any_io_executor;
        using protobuf_message_type = google::protobuf::Message;
        using void_awaitable_type = boost::asio::awaitable<void>;

        session() noexcept = default;

        virtual ~session() noexcept = default;

        session(const session& rhs) noexcept = default;

        session& operator=(const session& rhs) noexcept = default;

        session(session&& rhs) noexcept = default;

        session& operator=(session&& rhs) noexcept = default;

        virtual void stop() = 0;

        virtual void write(const header& header, const protobuf_message_type& response) = 0;

        [[nodiscard]] virtual void_awaitable_type write_immediately(const std::string& response) = 0;

        [[nodiscard]] virtual int64_t get_session_id() const noexcept = 0;

        virtual void remove_session() = 0;

        [[nodiscard]] virtual server_network_type get_server_network_type() const = 0;

        virtual void set_instance_id(const std::string& instance_id) = 0;

        [[nodiscard]] virtual std::string get_instance_id() const = 0;

        [[nodiscard]] virtual any_io_executor get_any_io_executor() = 0;

        [[nodiscard]] virtual std::string get_remote_ip_address() const = 0;
    };
}