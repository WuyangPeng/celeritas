#include "common/buffer/buffer_pool_data.h"
#include "common/core/celeritas_error.h"

#include <boost/test/unit_test.hpp>

#include <cstring>
#include <span>
#include <string>

namespace
{
    constexpr auto test_basic_size = 1024;
    constexpr auto test_operations_size = 100;

    void test_set_string(celeritas::buffer_pool_data& data)
    {
        const std::string test_str{ "hello" };
        data.set(test_str);

        BOOST_CHECK_EQUAL(std::strncmp(data.data(), test_str.c_str(), test_str.size()), 0);
        BOOST_CHECK_EQUAL(*data.get(0), 'h');
        BOOST_CHECK_EQUAL(*data.get(4), 'o');
    }

    void test_set_span(celeritas::buffer_pool_data& data)
    {
        std::string span_str{ "world" };
        const celeritas::buffer_pool_data::source_type span_source{ span_str };
        data.set(span_source);
        BOOST_CHECK_EQUAL(std::strncmp(data.data(), span_str.c_str(), span_str.size()), 0);
    }

    void test_error(celeritas::buffer_pool_data& data)
    {
        BOOST_CHECK_THROW(std::ignore = data.get(test_operations_size), celeritas::celeritas_error);
        BOOST_CHECK_THROW(std::ignore = data.get(test_operations_size + 1), celeritas:: celeritas_error);

        const std::string large(test_operations_size + 1, 'x');
        BOOST_CHECK_THROW(data.set(large), celeritas:: celeritas_error);
    }
}

BOOST_AUTO_TEST_SUITE(buffer_pool_data_suite)

    BOOST_AUTO_TEST_CASE(test_buffer_pool_data_basic)
    {
        celeritas::buffer_pool_data data{ test_basic_size };

        BOOST_CHECK_EQUAL(data.size(), test_basic_size);
        BOOST_CHECK(data.is_effective());
        BOOST_CHECK(data.data() != nullptr);

        const auto& const_data = data;
        BOOST_CHECK(const_data.data() != nullptr);
        BOOST_CHECK_EQUAL(const_data.size(), test_basic_size);
    }

    BOOST_AUTO_TEST_CASE(test_buffer_pool_data_empty)
    {
        celeritas::buffer_pool_data data{ 0 };
        BOOST_CHECK(!data.is_effective());
        BOOST_CHECK_EQUAL(data.size(), 0);

        BOOST_CHECK_THROW(std::ignore = data.data(), celeritas::celeritas_error);
        BOOST_CHECK_THROW(std::ignore = data.get(0), celeritas::celeritas_error);

        const auto& const_data = data;
        BOOST_CHECK_THROW(std::ignore = const_data.data(), celeritas:: celeritas_error);
    }

    BOOST_AUTO_TEST_CASE(test_buffer_pool_data_operations)
    {
        celeritas::buffer_pool_data data{ test_operations_size };

        test_set_string(data);
        test_set_span(data);
        test_error(data);
    }

BOOST_AUTO_TEST_SUITE_END()
