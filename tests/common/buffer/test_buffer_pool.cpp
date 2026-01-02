#include "common/buffer/buffer_pool.h"
#include "common/buffer/buffer_pool_data.h"

#include <boost/test/unit_test.hpp>

#include <chrono>
#include <string>

namespace
{
    constexpr auto test_acquire_release_size = 256;
    constexpr auto test_pool_reuse_size = 512;

    void test_reclaim()
    {
        celeritas::buffer_pool::reclaim(std::chrono::milliseconds(0));

        auto data = celeritas::buffer_pool::acquire(test_acquire_release_size);
        BOOST_CHECK_EQUAL(*data.get(0), '\0');
    }

    [[nodiscard]] const void* get_data_ptr()
    {
        auto data = celeritas::buffer_pool::acquire(test_pool_reuse_size);
        const void* ptr{ data.data() };
        celeritas::buffer_pool::release(std::move(data));

        return ptr;
    }
}

BOOST_AUTO_TEST_SUITE(buffer_pool_suite)

    BOOST_AUTO_TEST_CASE(test_buffer_pool_acquire_release)
    {
        auto data = celeritas::buffer_pool::acquire(test_acquire_release_size);

        BOOST_CHECK(data.size() >= test_acquire_release_size);
        BOOST_CHECK(data.is_effective());

        data.set(std::string{ "pool" });

        celeritas::buffer_pool::release(std::move(data));

        BOOST_CHECK(!data.is_effective());

        test_reclaim();
    }

    BOOST_AUTO_TEST_CASE(test_buffer_pool_reuse)
    {
        const auto* ptr = get_data_ptr();

        auto data = celeritas::buffer_pool::acquire(test_pool_reuse_size);
        BOOST_CHECK(data.size() >= test_pool_reuse_size);
        BOOST_CHECK_EQUAL(data.data(), ptr);
        celeritas::buffer_pool::release(std::move(data));
    }

BOOST_AUTO_TEST_SUITE_END()
