#include "common/buffer/buffer_guard.h"
#include "common/buffer/buffer_pool.h"
#include "common/core/celeritas_error.h"

#include <boost/test/unit_test.hpp>

#include <cstring>
#include <span>
#include <string>

BOOST_AUTO_TEST_SUITE(buffer_guard_suite)

    BOOST_AUTO_TEST_CASE(buffer_guard_lifecycle_test)
    {
        constexpr auto size = 128;
        auto data = celeritas::buffer_pool::acquire(size);

        {
            celeritas::buffer_guard guard{ std::move(data), 0 };
            BOOST_CHECK(guard.is_effective());
            BOOST_CHECK(guard.size() >= size);

            guard.set_effective_size(50);
            BOOST_CHECK_EQUAL(guard.get_effective_size(), 50);

            guard.set(std::string{ "guard" });
            BOOST_CHECK_EQUAL(std::strncmp(guard.get(), "guard", 5), 0);

            auto guard2 = std::move(guard);
            BOOST_CHECK(guard2.is_effective());
            BOOST_CHECK(!guard.is_effective());
            BOOST_CHECK_EQUAL(guard2.get_effective_size(), 50);

            celeritas::buffer_guard guard3{ celeritas::buffer_pool::acquire(10), 0 };
            guard3 = std::move(guard2);
            BOOST_CHECK(guard3.is_effective());
            BOOST_CHECK(!guard2.is_effective());
            BOOST_CHECK_EQUAL(guard3.get_effective_size(), 50);
        }
    }

    BOOST_AUTO_TEST_CASE(buffer_guard_reuse_test)
    {
        constexpr auto size = 512;
        const void* ptr = nullptr;

        {
            auto data = celeritas::buffer_pool::acquire(size);
            ptr = data.data();
            celeritas::buffer_guard guard(std::move(data), 0);
        }

        {
            auto data = celeritas::buffer_pool::acquire(size);
            BOOST_CHECK_EQUAL(data.data(), ptr);
            celeritas::buffer_pool::release(std::move(data));
        }
    }

    BOOST_AUTO_TEST_CASE(buffer_guard_access_test)
    {
        constexpr auto size = 100;
        celeritas::buffer_pool_data data{ size };
        celeritas::buffer_guard guard{ std::move(data), 0 };

        guard.set(std::string("test"));
        BOOST_CHECK_EQUAL(*guard.get(0), 't');
        BOOST_CHECK_EQUAL(*guard.get(1), 'e');

        std::string span_str{ "span" };
        const celeritas::buffer_guard::source_type span_source(span_str);
        guard.set(span_source);
        BOOST_CHECK_EQUAL(std::strncmp(guard.get(), "span", 4), 0);

        BOOST_CHECK_THROW([&] { std::ignore = guard.get(size); }(), celeritas::celeritas_error);
    }

    BOOST_AUTO_TEST_CASE(buffer_guard_empty_test)
    {
        celeritas::buffer_pool_data empty_data{ 0 };
        const celeritas::buffer_guard guard{ std::move(empty_data), 0 };

        BOOST_CHECK(!guard.is_effective());
    }

BOOST_AUTO_TEST_SUITE_END()
