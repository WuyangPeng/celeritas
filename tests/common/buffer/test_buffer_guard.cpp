#include "common/buffer/buffer_guard.h"
#include "common/buffer/buffer_pool.h"
#include "common/core/celeritas_error.h"

#include <boost/test/unit_test.hpp>

#include <cstring>
#include <span>
#include <string>

namespace
{
    constexpr auto lifecycle_test_size = 128;
    constexpr auto reuse_test_size = 512;
    constexpr auto access_test_size = 100;

    void check_guard_state(const celeritas::buffer_guard& guard,
                           const bool is_effective,
                           const size_t effective_size = 0)
    {
        BOOST_CHECK_EQUAL(guard.is_effective(), is_effective);
        if (is_effective)
        {
            BOOST_CHECK_EQUAL(guard.get_effective_size(), effective_size);
        }
    }

    void check_content(celeritas::buffer_guard& guard, const std::string& content)
    {
        guard.set(content);
        BOOST_CHECK_EQUAL(std::strncmp(guard.get(), content.c_str(), content.size()), 0);
    }

    void check_buffer_guard_lifecycle(celeritas::buffer_guard& guard)
    {
        BOOST_CHECK(guard.is_effective());
        BOOST_CHECK(guard.size() >= lifecycle_test_size);

        guard.set_effective_size(50);
        check_guard_state(guard, true, 50);

        check_content(guard, "guard");
    }

    void check_buffer_guard_access(celeritas::buffer_guard& guard)
    {
        check_content(guard, "test");
        BOOST_CHECK_EQUAL(*guard.get(0), 't');
        BOOST_CHECK_EQUAL(*guard.get(1), 'e');

        std::string span_str{ "span" };
        const celeritas::buffer_guard::source_type span_source{ span_str };
        guard.set(span_source);
        BOOST_CHECK_EQUAL(std::strncmp(guard.get(), "span", 4), 0);

        BOOST_CHECK_THROW(std::ignore = guard.get(access_test_size);, celeritas::celeritas_error);
    }

    [[nodiscard]] const void* get_data_ptr()
    {
        auto data = celeritas::buffer_pool::acquire(reuse_test_size);
        const void* ptr{ data.data() };
        celeritas::buffer_guard guard(std::move(data), 0);

        return ptr;
    }
}

BOOST_AUTO_TEST_SUITE(buffer_guard_suite)

    BOOST_AUTO_TEST_CASE(test_buffer_guard_lifecycle)
    {
        auto data = celeritas::buffer_pool::acquire(lifecycle_test_size);

        celeritas::buffer_guard guard1{ std::move(data), 0 };
        check_buffer_guard_lifecycle(guard1);

        auto guard2 = std::move(guard1);
        check_guard_state(guard2, true, 50);
        check_guard_state(guard1, false);

        celeritas::buffer_guard guard3{ celeritas::buffer_pool::acquire(10), 0 };
        guard3 = std::move(guard2);
        check_guard_state(guard3, true, 50);
        check_guard_state(guard2, false);
    }

    BOOST_AUTO_TEST_CASE(test_buffer_guard_reuse)
    {
        const auto* ptr = get_data_ptr();

        auto data = celeritas::buffer_pool::acquire(reuse_test_size);
        BOOST_CHECK_EQUAL(data.data(), ptr);
        celeritas::buffer_pool::release(std::move(data));
    }

    BOOST_AUTO_TEST_CASE(test_buffer_guard_access)
    {
        celeritas::buffer_pool_data data{ access_test_size };
        celeritas::buffer_guard guard{ std::move(data), 0 };

        check_buffer_guard_access(guard);
    }

    BOOST_AUTO_TEST_CASE(test_buffer_guard_empty)
    {
        celeritas::buffer_pool_data empty_data{ 0 };
        const celeritas::buffer_guard guard{ std::move(empty_data), 0 };

        check_guard_state(guard, false);
    }

BOOST_AUTO_TEST_SUITE_END()