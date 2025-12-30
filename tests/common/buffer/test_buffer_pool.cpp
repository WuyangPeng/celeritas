#include "common/buffer/buffer_pool.h"
#include "common/buffer/buffer_pool_data.h"

#include <boost/test/unit_test.hpp>

#include <chrono>
#include <string>

BOOST_AUTO_TEST_SUITE(buffer_pool_suite)

    BOOST_AUTO_TEST_CASE(test_buffer_pool_acquire_release)
    {
        constexpr auto size = 256;
        auto data = celeritas::buffer_pool::acquire(size);

        BOOST_CHECK(data.size() >= size);
        BOOST_CHECK(data.is_effective());

        data.set(std::string("pool"));

        celeritas::buffer_pool::release(std::move(data));

        BOOST_CHECK(!data.is_effective());

        celeritas::buffer_pool::reclaim(std::chrono::seconds(1));
    }

    BOOST_AUTO_TEST_CASE(test_buffer_pool_reuse)
    {
        constexpr auto size = 512;
        const void* ptr = nullptr;

        {
            auto data1 = celeritas::buffer_pool::acquire(size);
            ptr = data1.data();
            celeritas::buffer_pool::release(std::move(data1));
        }

        {
            auto data2 = celeritas::buffer_pool::acquire(size);
            BOOST_CHECK(data2.size() >= size);
            BOOST_CHECK_EQUAL(data2.data(), ptr);
            celeritas::buffer_pool::release(std::move(data2));
        }
    }

BOOST_AUTO_TEST_SUITE_END()
