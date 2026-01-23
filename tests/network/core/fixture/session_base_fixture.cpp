#include "session_base_fixture.h"
#include "common/core/noexcept_safe_call_and_log.h"
#include "config/basic/server_network_type.h"

#include <boost/asio/detached.hpp>
#include <boost/test/unit_test.hpp>

celeritas::session_base_fixture::session_base_fixture()
    : io_context_{},
      listener_sessions_{ std::make_shared<mock_listener_sessions>() },
      callback_{ listener_sessions_, std::weak_ptr<network_message_callback>() },
      session_{ std::make_shared<mock_session_base>(io_context_.get_executor(), server_network_type::tcp, 12345, callback_) },
      test_end_{ false }
{
}

void celeritas::session_base_fixture::set_test_end(const bool test_end)
{
    test_end_ = test_end;
}

void celeritas::session_base_fixture::run(awaitable_function test_body)
{
    boost::asio::co_spawn(io_context_,
                          noexcept_safe_call_and_log_awaitable(std::move(test_body),
                                                               celeritas::database_channel,
                                                               "database session test run error: "),
                          boost::asio::detached);

    io_context_.run();
    io_context_.restart();

    BOOST_CHECK(test_end_);
}

celeritas::session_base_fixture::mock_session_base_shared_ptr celeritas::session_base_fixture::get_session() const
{
    return session_;
}

celeritas::session_base_fixture::mock_listener_sessions_shared_ptr celeritas::session_base_fixture::get_listener_sessions() const
{
    return listener_sessions_;
}
