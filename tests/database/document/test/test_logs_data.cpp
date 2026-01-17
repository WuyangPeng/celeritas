#include "database/basic/basis_database.tpp"
#include "database/detail/check_helper.h"
#include "database/document/test/logs_data.h"

#include <boost/test/unit_test.hpp>

#include <algorithm>
#include <string>
#include <vector>

namespace
{
    [[nodiscard]] celeritas::logs_data get_test_logs_data()
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

        const celeritas::logs_data::document_type nested_document{ celeritas::basis_database{ "nested_int", 100 },
                                                                   celeritas::basis_database{ "nested_string", "hello" } };

        data.set_document_value(nested_document);

        celeritas::logs_data::document_type document_array_element1{ celeritas::basis_database{ "array_int1", 1 } };
        celeritas::logs_data::document_type document_array_element2{ celeritas::basis_database{ "array_string2", "world" } };

        data.set_document_array_value({ document_array_element1, document_array_element2 });

        return data;
    }

    [[nodiscard]] const celeritas::basis_database& get_field_value(const celeritas::logs_data::document_type& document, std::string_view field_name)
    {
        const auto iter = std::ranges::find_if(document, [field_name](const auto& field) {
            return field.get_field_name() == field_name;
        });
        BOOST_REQUIRE_MESSAGE(iter != document.cend(), "Field '" << field_name << "' not found");
        return *iter;
    }

    void verify_document_data(const celeritas::logs_data::document_type& document, const celeritas::logs_data& original_data)
    {
        BOOST_CHECK_EQUAL(get_field_value(document, celeritas::logs_data::int64_value_description).get_value<celeritas::database_data_type::int64_type>(),
                          original_data.get_int64_value());
        BOOST_CHECK_EQUAL(get_field_value(document, celeritas::logs_data::string_value_description).get_value<celeritas::database_data_type::string_type>(),
                          original_data.get_string_value());
        celeritas::check_array(get_field_value(document, celeritas::logs_data::string_array_value_description).get_value<celeritas::database_data_type::string_array_type>(),
                               original_data.get_string_array_value());
        BOOST_CHECK_EQUAL(get_field_value(document, celeritas::logs_data::int32_value_description).get_value<celeritas::database_data_type::int32_type>(),
                          original_data.get_int32_value());
        BOOST_CHECK_EQUAL(get_field_value(document, celeritas::logs_data::int32_count_value_description).get_value<celeritas::database_data_type::int32_count_type>(),
                          original_data.get_int32_count_value());
        celeritas::check_array(get_field_value(document, celeritas::logs_data::int32_array_value_description).get_value<celeritas::database_data_type::int32_array_type>(),
                               original_data.get_int32_array_value());
        BOOST_CHECK_EQUAL(get_field_value(document, celeritas::logs_data::int64_count_value_description).get_value<celeritas::database_data_type::int64_count_type>(),
                          original_data.get_int64_count_value());
        celeritas::check_array(get_field_value(document, celeritas::logs_data::int64_array_value_description).get_value<celeritas::database_data_type::int64_array_type>(),
                               original_data.get_int64_array_value());
        BOOST_CHECK_CLOSE(get_field_value(document, celeritas::logs_data::double_value_description).get_value<celeritas::database_data_type::double_type>(),
                          original_data.get_double_value(), 0.001);
        celeritas::check_array(get_field_value(document, celeritas::logs_data::double_array_value_description).get_value<celeritas::database_data_type::double_array_type>(),
                               original_data.get_double_array_value());
        BOOST_CHECK_EQUAL(get_field_value(document, celeritas::logs_data::bool_value_description).get_value<celeritas::database_data_type::bool_type>(),
                          original_data.get_bool_value());
        BOOST_CHECK(get_field_value(document, celeritas::logs_data::document_value_description).get_value<celeritas::database_data_type::document_type>() == original_data.get_document_value());
        BOOST_CHECK(get_field_value(document, celeritas::logs_data::document_array_value_description).get_value<celeritas::database_data_type::document_array_type>() == original_data.get_document_array_value());
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
        BOOST_CHECK(data.get_document_value().empty());
        BOOST_CHECK(data.get_document_array_value().empty());
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

        celeritas::check_array(data.get_string_array_value(), test_value);
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

        celeritas::check_array(data.get_int32_array_value(), test_value);
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

        celeritas::check_array(data.get_int64_array_value(), test_value);
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
        celeritas::check_array(data.get_double_array_value(), test_value);
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

        const celeritas::logs_data::document_type test_value{ celeritas::basis_database{ "int_field", 12345 },
                                                              celeritas::basis_database{ "string_field", "test_string" },
                                                              celeritas::basis_database{ "bool_field", true } };

        data.set_document_value(test_value);

        const auto& retrieved_value = data.get_document_value();

        BOOST_CHECK(retrieved_value == test_value);
    }

    BOOST_AUTO_TEST_CASE(test_document_array_value_accessors)
    {
        const celeritas::logs_data::document_type doc1{ celeritas::basis_database{ "field1", 1 },
                                                        celeritas::basis_database{ "field2", "value1" } };

        const celeritas::logs_data::document_type doc2{ celeritas::basis_database{ "field3", 2 },
                                                        celeritas::basis_database{ "field4", "value2" } };

        const celeritas::logs_data::document_array_type test_value{ doc1, doc2 };

        celeritas::logs_data data{};
        data.set_document_array_value(test_value);

        const auto& retrieved_value = data.get_document_array_value();
        BOOST_CHECK_EQUAL(retrieved_value.size(), test_value.size());
        BOOST_CHECK(retrieved_value == test_value);
    }

    BOOST_AUTO_TEST_CASE(test_to_document_type)
    {
        const auto original_data = get_test_logs_data();
        const auto document = original_data.to_document_type();

        verify_document_data(document, original_data);
    }

    BOOST_AUTO_TEST_CASE(test_from_document)
    {
        const auto original_data = get_test_logs_data();
        const auto document = original_data.to_document_type();

        const auto loaded_data = celeritas::logs_data::from_document(document);

        check_logs_data(original_data, loaded_data);
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
