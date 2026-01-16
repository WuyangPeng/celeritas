#include "database/document/test/logs_data.h"

#include <boost/test/unit_test.hpp>

#include <string>
#include <type_traits>
#include <vector>

namespace
{
    template <typename ArrayType>
    void verify_array_values(const ArrayType& lhs, const ArrayType& rhs)
    {
        BOOST_CHECK_EQUAL(lhs.size(), rhs.size());
        if (lhs.size() != rhs.size())
        {
            return;
        }

        for (auto i = 0; i < lhs.size(); ++i)
        {
            if constexpr (std::is_floating_point_v<typename ArrayType::value_type>)
            {
                BOOST_CHECK_CLOSE(lhs.at(i), rhs.at(i), 0.001);
            }
            else
            {
                BOOST_CHECK_EQUAL(lhs.at(i), rhs.at(i));
            }
        }
    }
}

BOOST_AUTO_TEST_SUITE(logs_data_suite)

    BOOST_AUTO_TEST_CASE(test_default_constructor)
    {
        const celeritas::logs_data data{};

        BOOST_CHECK_EQUAL(data.get_int64_value(), 0);
        BOOST_CHECK_EQUAL(data.get_string_value(), "");
        BOOST_CHECK_EQUAL(data.get_int32_value(), 0);
        BOOST_CHECK_EQUAL(data.get_int32_count_value(), 0);
        BOOST_CHECK_EQUAL(data.get_int64_count_value(), 0);
        BOOST_CHECK_CLOSE(data.get_double_value(), 0.0, 0.000001);
        BOOST_CHECK_EQUAL(data.get_bool_value(), false);
    }

    BOOST_AUTO_TEST_CASE(test_int64_value_accessors)
    {
        celeritas::logs_data data{};
        constexpr auto test_value = 123456789LL;

        data.set_int64_value(test_value);

        BOOST_CHECK_EQUAL(data.get_int64_value(), test_value);
    }

    BOOST_AUTO_TEST_CASE(test_string_value_accessors)
    {
        celeritas::logs_data data{};
        const auto test_value = "test_string_value";

        data.set_string_value(test_value);

        BOOST_CHECK_EQUAL(data.get_string_value(), test_value);
    }

    BOOST_AUTO_TEST_CASE(test_string_array_value_accessors)
    {
        celeritas::logs_data data{};
        const celeritas::logs_data::string_array_type test_value{ "item1", "item2", "item3" };

        data.set_string_array_value(test_value);

        verify_array_values(data.get_string_array_value(), test_value);
    }

    BOOST_AUTO_TEST_CASE(test_int32_value_accessors)
    {
        celeritas::logs_data data{};
        constexpr auto test_value = 987654;

        data.set_int32_value(test_value);

        BOOST_CHECK_EQUAL(data.get_int32_value(), test_value);
    }

    BOOST_AUTO_TEST_CASE(test_int32_count_value_accessors)
    {
        celeritas::logs_data data{};
        constexpr auto test_value = 500;

        data.set_int32_count_value(test_value);

        BOOST_CHECK_EQUAL(data.get_int32_count_value(), test_value);
    }

    BOOST_AUTO_TEST_CASE(test_int32_array_value_accessors)
    {
        celeritas::logs_data data{};
        const celeritas::logs_data::int32_array_type test_value{ 10, 20, 30, 40 };

        data.set_int32_array_value(test_value);

        verify_array_values(data.get_int32_array_value(), test_value);
    }

    BOOST_AUTO_TEST_CASE(test_int64_count_value_accessors)
    {
        celeritas::logs_data data{};
        constexpr auto test_value = 999888777LL;

        data.set_int64_count_value(test_value);

        BOOST_CHECK_EQUAL(data.get_int64_count_value(), test_value);
    }

    BOOST_AUTO_TEST_CASE(test_int64_array_value_accessors)
    {
        celeritas::logs_data data{};
        const celeritas::logs_data::int64_array_type test_value{ 100LL, 200LL, 300LL };

        data.set_int64_array_value(test_value);

        verify_array_values(data.get_int64_array_value(), test_value);
    }

    BOOST_AUTO_TEST_CASE(test_double_value_accessors)
    {
        celeritas::logs_data data{};
        constexpr auto test_value = 3.14159;

        data.set_double_value(test_value);

        BOOST_CHECK_CLOSE(data.get_double_value(), test_value, 0.001);
    }

    BOOST_AUTO_TEST_CASE(test_double_array_value_accessors)
    {
        celeritas::logs_data data{};
        const celeritas::logs_data::double_array_type test_value{ 1.1, 2.2, 3.3 };

        data.set_double_array_value(test_value);
        verify_array_values(data.get_double_array_value(), test_value);
    }

    BOOST_AUTO_TEST_CASE(test_bool_value_accessors)
    {
        celeritas::logs_data data{};

        data.set_bool_value(true);
        BOOST_CHECK_EQUAL(data.get_bool_value(), true);

        data.set_bool_value(false);
        BOOST_CHECK_EQUAL(data.get_bool_value(), false);
    }

    BOOST_AUTO_TEST_CASE(test_document_value_accessors)
    {
        celeritas::logs_data data{};

        const celeritas::logs_data::document_type test_value{};

        data.set_document_value(test_value);

        const auto& retrieved_value = data.get_document_value();

        (void)retrieved_value;
    }

    BOOST_AUTO_TEST_CASE(test_document_array_value_accessors)
    {
        celeritas::logs_data data{};
        const celeritas::logs_data::document_array_type test_value{};

        data.set_document_array_value(test_value);

        const auto& retrieved_value = data.get_document_array_value();
        BOOST_CHECK_EQUAL(retrieved_value.size(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_to_document_type)
    {
        celeritas::logs_data data{};
        data.set_int64_value(123456LL);
        data.set_string_value("test_string");
        data.set_bool_value(true);
        data.set_double_value(2.718);

        const auto document = data.to_document_type();

        (void)document;
    }

    BOOST_AUTO_TEST_CASE(test_from_document)
    {
        const celeritas::logs_data::document_type doc{};

        const auto data = celeritas::logs_data::from_document(doc);

        BOOST_CHECK_EQUAL(data.get_int64_value(), 0);
        BOOST_CHECK_EQUAL(data.get_string_value(), "");
        BOOST_CHECK_EQUAL(data.get_bool_value(), false);
    }

    BOOST_AUTO_TEST_CASE(test_complete_data_flow)
    {
        celeritas::logs_data data{};

        data.set_int64_value(111111LL);
        data.set_string_value("complete_test_string");
        data.set_string_array_value({ "str1", "str2", "str3" });
        data.set_int32_value(222);
        data.set_int32_count_value(333);
        data.set_int32_array_value({ 444, 555, 666 });
        data.set_int64_count_value(777777LL);
        data.set_int64_array_value({ 888888LL, 999999LL });
        data.set_double_value(1.2345);
        data.set_double_array_value({ 6.7, 8.9, 10.11 });
        data.set_bool_value(true);

        BOOST_CHECK_EQUAL(data.get_int64_value(), 111111LL);
        BOOST_CHECK_EQUAL(data.get_string_value(), "complete_test_string");
        BOOST_CHECK_EQUAL(data.get_string_array_value().size(), 3);
        BOOST_CHECK_EQUAL(data.get_int32_value(), 222);
        BOOST_CHECK_EQUAL(data.get_int32_count_value(), 333);
        BOOST_CHECK_EQUAL(data.get_int32_array_value().size(), 3);
        BOOST_CHECK_EQUAL(data.get_int64_count_value(), 777777LL);
        BOOST_CHECK_EQUAL(data.get_int64_array_value().size(), 2);
        BOOST_CHECK_CLOSE(data.get_double_value(), 1.2345, 0.001);
        BOOST_CHECK_EQUAL(data.get_double_array_value().size(), 3);
        BOOST_CHECK_EQUAL(data.get_bool_value(), true);
    }

BOOST_AUTO_TEST_SUITE_END()
