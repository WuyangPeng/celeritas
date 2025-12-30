#include "common/buffer/buffer_pool_data.h"
#include "common/core/celeritas_error.h"

#include <boost/test/unit_test.hpp>

#include <cstring>
#include <span>
#include <string>

BOOST_AUTO_TEST_SUITE(buffer_pool_data_suite)

    BOOST_AUTO_TEST_CASE(buffer_pool_data_basic_test)
    {
        constexpr auto size = 1024;
        celeritas::buffer_pool_data data{ size };

        BOOST_CHECK_EQUAL(data.size(), size);
        BOOST_CHECK(data.is_effective());
        BOOST_CHECK(data.data() != nullptr);

        const auto& const_data = data;
        BOOST_CHECK(const_data.data() != nullptr);
        BOOST_CHECK_EQUAL(const_data.size(), size);
    }

    BOOST_AUTO_TEST_CASE(buffer_pool_data_empty_test)
    {
        celeritas::buffer_pool_data data{ 0 };
        BOOST_CHECK(!data.is_effective());
        BOOST_CHECK_EQUAL(data.size(), 0);

        BOOST_CHECK_THROW([&data] { std::ignore = data.data(); }(), celeritas::celeritas_error);
        BOOST_CHECK_THROW([&data] { std::ignore = data.get(0); }(), celeritas::celeritas_error);

        const auto& const_data = data;
        BOOST_CHECK(const_data.data() == nullptr);
    }

    BOOST_AUTO_TEST_CASE(buffer_pool_data_operations_test)
    {
        constexpr auto size = 100;
        celeritas::buffer_pool_data data{ size };

        const std::string test_str{ "hello" };
        data.set(test_str);

        BOOST_CHECK_EQUAL(std::strncmp(data.data(), test_str.c_str(), test_str.size()), 0);
        BOOST_CHECK_EQUAL(*data.get(0), 'h');
        BOOST_CHECK_EQUAL(*data.get(4), 'o');

        std::string span_str{ "world" };
        const celeritas::buffer_pool_data::source_type span_source(span_str);
        data.set(span_source);
        BOOST_CHECK_EQUAL(std::strncmp(data.data(), span_str.c_str(), span_str.size()), 0);

        BOOST_CHECK_THROW([&data] { std::ignore =data.get(size); }(), celeritas::celeritas_error);
        BOOST_CHECK_THROW([&data] { std::ignore =data.get(size + 1); }(), celeritas:: celeritas_error);

        const std::string large_str(size + 1, 'x');
        BOOST_CHECK_THROW(data.set(large_str), celeritas:: celeritas_error);
    }

BOOST_AUTO_TEST_SUITE_END()
