#pragma once

#include "network/core/mock/mock_listener_sessions.h"
#include "network/core/mock/mock_session_base.h"
#include "network/session_helper/session_callback.h"

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/io_context.hpp>

#include <functional>
#include <memory>

namespace celeritas
{
    class session_base_fixture
    {
    public:
        using class_type = session_base_fixture;
        using awaitable_function = std::function<boost::asio::awaitable<void>()>;
        using mock_session_base_shared_ptr = std::shared_ptr<mock_session_base>;
        using mock_listener_sessions_shared_ptr = std::shared_ptr<mock_listener_sessions>;

        session_base_fixture();

    protected:
        void set_test_end(bool test_end);

        void run(awaitable_function test_body);

        [[nodiscard]] mock_session_base_shared_ptr get_session() const;

        [[nodiscard]] mock_listener_sessions_shared_ptr get_listener_sessions() const;

    private:
        boost::asio::io_context io_context_;
        mock_listener_sessions_shared_ptr listener_sessions_;
        session_callback callback_;
        mock_session_base_shared_ptr session_;
        bool test_end_;
    };
}
