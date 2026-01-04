#include "common/framework/timer_base.h"
#include "mock/mock_timer.h"

#include <boost/test/unit_test.hpp>
#include <boost/asio.hpp>

#include <chrono>
#include <thread>

BOOST_AUTO_TEST_SUITE(timer_base_suite)

    BOOST_AUTO_TEST_CASE(test_timer_execution)
    {
        boost::asio::io_context io_context{};
        const auto timer = std::make_shared<celeritas::mock_timer>(boost::asio::make_strand(io_context), std::chrono::milliseconds(100), false);

        timer->start();

        io_context.run_for(std::chrono::milliseconds(250));

        BOOST_CHECK(timer->get_execution_count() > 1);
        timer->stop();
    }

    BOOST_AUTO_TEST_CASE(test_timer_stop)
    {
        boost::asio::io_context io_context{};
        const auto timer = std::make_shared<celeritas::mock_timer>(boost::asio::make_strand(io_context), std::chrono::milliseconds(10), false);

        timer->start();
        io_context.poll();
        timer->stop();

        const auto count_after_stop = timer->get_execution_count();

        io_context.run_for(std::chrono::milliseconds(25));

        BOOST_CHECK_EQUAL(timer->get_execution_count(), count_after_stop);
    }

    BOOST_AUTO_TEST_CASE(test_disposable_timer)
    {
        boost::asio::io_context io_context{};
        const auto timer = std::make_shared<celeritas::mock_timer>(boost::asio::make_strand(io_context), std::chrono::milliseconds(10), true);

        timer->start();

        io_context.run_for(std::chrono::milliseconds(25));

        BOOST_CHECK_EQUAL(timer->get_execution_count(), 1);
        timer->stop();
    }

    BOOST_AUTO_TEST_CASE(test_start_with_immediate_execution)
    {
        boost::asio::io_context io_context{};
        const auto timer = std::make_shared<celeritas::mock_timer>(boost::asio::make_strand(io_context), std::chrono::milliseconds(10), false);

        BOOST_CHECK_EQUAL(timer->get_execution_count(), 0);
        timer->start(true);

        BOOST_CHECK_EQUAL(timer->get_execution_count(), 1);

        io_context.run_for(std::chrono::milliseconds(25));

        BOOST_CHECK(timer->get_execution_count() > 1);
        timer->stop();
    }

BOOST_AUTO_TEST_SUITE_END()
