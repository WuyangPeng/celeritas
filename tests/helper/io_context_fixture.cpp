#include "io_context_fixture.h"
#include "common/core/noexcept_safe_call_and_log.h"

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/test/unit_test.hpp>

celeritas::io_context_fixture::io_context_fixture()
    : io_context_{}, test_end_{ false }
{
}

celeritas::io_context_fixture::~io_context_fixture() noexcept
{
    noexcept_safe_call_and_log([this] {
                                   io_context_.stop();;
                               },
                               config_channel,
                               "io context fixture stop error: ");
}

void celeritas::io_context_fixture::spawn(awaitable_function function)
{
    boost::asio::co_spawn(io_context_,
                          noexcept_safe_call_and_log_awaitable(std::move(function),
                                                               default_channel,
                                                               "io context fixture run error: "),
                          boost::asio::detached);
}

void celeritas::io_context_fixture::run(awaitable_function function)
{
    spawn(std::move(function));

    io_context_.run();
    io_context_.restart();

    BOOST_CHECK(test_end_);
}

void celeritas::io_context_fixture::set_test_end(const bool test_end)
{
    test_end_ = test_end;
}

