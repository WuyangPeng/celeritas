#pragma once

#include "network/session_helper/session_callback.h"
#include "network/session_helper/session_run.h"

#include <boost/asio.hpp>
#include <boost/url.hpp>

namespace celeritas
{
    class http_session_run : public session_run
    {
    public:
        using class_type = http_session_run;
        using base_type = session_run;
        using socket_type = boost::asio::ip::tcp::socket;

        http_session_run(socket_type& socket, int64_t session_id, session_callback session_callback);

        void do_start() override;

        [[nodiscard]] void_awaitable_type run() override;

    protected:
        [[nodiscard]] void_awaitable_type handle_one_request_message();

        [[nodiscard]] void_awaitable_type handle_one_response_message(const std::string& path);

    private:
        using urls_params_view_type = boost::urls::params_view;

        void close_socket();

        [[nodiscard]] virtual void_awaitable_type handle_one_message() = 0;

        void call_back(const std::string& path, const urls_params_view_type& params);

        void call_back(const std::string& path, const std::string& params);

        socket_type& socket_;
        int64_t session_id_;
        session_callback session_callback_;
    };
}