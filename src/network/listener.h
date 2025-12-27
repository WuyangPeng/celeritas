#pragma once

#include "network_fwd.h"
#include "config/config_fwd.h"
#include "message/message_fwd.h"

#include <boost/asio.hpp>
#include <google/protobuf/message.h>

#include <memory>

namespace celeritas
{
    class listener : public std::enable_shared_from_this<listener>
    {
    public:
        using class_type = listener;
        using io_context_type = boost::asio::io_context;
        using network_message_callback_weak_ptr = std::weak_ptr<network_message_callback>;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using session_shared_ptr = std::shared_ptr<session_base>;
        using protobuf_message = google::protobuf::Message;

        listener(io_context_type& io_context,
                 network_message_callback_weak_ptr callback,
                 std::string game_server_id,
                 server_network_type server_network_type);

        virtual ~listener() noexcept = default;

        listener(const listener& rhs) = delete;

        listener& operator=(const listener& rhs) = delete;

        listener(listener&& rhs) noexcept = delete;

        listener& operator=(listener&& rhs) noexcept = delete;

        virtual void stop() = 0;

        // 开始监听新连接
        void start();

        // 协程：异步接受新连接
        [[nodiscard]] virtual void_awaitable_type accept_connections() = 0;

        [[nodiscard]] virtual session_shared_ptr get_session(int64_t id) = 0;

        [[nodiscard]] server_network_type get_server_network_type() const noexcept;

        [[nodiscard]] virtual bool write(const std::string& server_type, const std::string& instance_id, const header& header, const protobuf_message& request) = 0;

    protected:
        [[nodiscard]] std::string get_game_server_id() const;

        [[nodiscard]] network_message_callback_weak_ptr get_network_message_callback();

        [[nodiscard]] io_context_type& get_io_context();

    private:
        io_context_type& io_context_;
        network_message_callback_weak_ptr network_message_callback_;
        std::string game_server_id_;
        server_network_type server_network_type_;
    };
}

