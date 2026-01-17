#include "concurrent_access_tester.h"

#include "database/config/config_manager.h"
#include "database/config/mock/mock_config_database_pool.h"

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/test/unit_test.hpp>

celeritas::concurrent_access_tester::concurrent_access_tester(const any_io_executor& executor)
    : any_io_executor_{ executor },
      tasks_remaining_{ std::make_shared<std::atomic_int>(0) },
      exception_{ std::make_shared<std::exception_ptr>() }
{
}

void celeritas::concurrent_access_tester::run()
{
    constexpr auto task_count = 10;
    tasks_remaining_->store(task_count);

    for (auto i = 0; i < task_count; ++i)
    {
        boost::asio::co_spawn(any_io_executor_, check_time_refresh_task(),
                              [tasks_remaining = tasks_remaining_, exception = exception_](const std::exception_ptr& ptr) {
                                  if (ptr && !*exception)
                                  {
                                      *exception = ptr;
                                  }
                                  --(*tasks_remaining);
                              });
    }
}

celeritas::concurrent_access_tester::void_awaitable_type celeritas::concurrent_access_tester::wait()
{
    while (tasks_remaining_->load() > 0)
    {
        co_await boost::asio::steady_timer(any_io_executor_, std::chrono::milliseconds{ 1 }).async_wait(boost::asio::use_awaitable);
    }

    if (*exception_)
    {
        std::rethrow_exception(*exception_);
    }
}

celeritas::concurrent_access_tester::void_awaitable_type celeritas::concurrent_access_tester::check_time_refresh_task()
{
    if (const auto time_refresh_opt = celeritas::config_manager::get_instance().get_time_refresh(celeritas::mock_config_database_pool::time_refresh_id);
        time_refresh_opt.has_value())
    {
        BOOST_CHECK_EQUAL(time_refresh_opt->get_time_refresh_type(), 2);
        BOOST_CHECK_EQUAL(time_refresh_opt->get_parameter(), 3);
    }
    else
    {
        BOOST_FAIL("time_refresh_opt should have a value after loading");
    }
    co_return;
}

