#include "database/basic/basis_database.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(basis_database_suite)

    BOOST_AUTO_TEST_CASE(test_basis_database_string_constructor)
    {
        const std::string field_name{ "name" };
        const std::string value{ "test_name" };
        const celeritas::basis_database db{ field_name, value };

        BOOST_CHECK_EQUAL(db.get_field_name(), field_name);
        BOOST_CHECK(db.get_data_type() == celeritas::database_data_type::string_type);
        BOOST_CHECK_EQUAL(db.get_value<celeritas::database_data_type::string_type>(), value);
        BOOST_CHECK_EQUAL(db.get_string(), "'test_name'");
    }

    BOOST_AUTO_TEST_CASE(test_basis_database_const_char_ptr_constructor)
    {
        const std::string field_name{ "name" };
        const auto value = "test_name";
        const celeritas::basis_database db{ field_name, value };

        BOOST_CHECK_EQUAL(db.get_field_name(), field_name);
        BOOST_CHECK(db.get_data_type() == celeritas::database_data_type::string_type);
        BOOST_CHECK_EQUAL(db.get_value<celeritas::database_data_type::string_type>(), value);
    }

    BOOST_AUTO_TEST_CASE(test_basis_database_int32_constructor)
    {
        const std::string field_name{ "age" };
        constexpr auto value = 30;
        const celeritas::basis_database db{ field_name, value };

        BOOST_CHECK_EQUAL(db.get_field_name(), field_name);
        BOOST_CHECK(db.get_data_type() == celeritas::database_data_type::int32_type);
        BOOST_CHECK_EQUAL(db.get_value<celeritas::database_data_type::int32_type>(), value);
        BOOST_CHECK_EQUAL(db.get_string(), "30");
    }

    BOOST_AUTO_TEST_CASE(test_basis_database_int64_constructor)
    {
        const std::string field_name{ "id" };
        constexpr auto value = 123456789012345;
        const celeritas::basis_database db{ field_name, value };

        BOOST_CHECK_EQUAL(db.get_field_name(), field_name);
        BOOST_CHECK(db.get_data_type() == celeritas::database_data_type::int64_type);
        BOOST_CHECK_EQUAL(db.get_value<celeritas::database_data_type::int64_type>(), value);
        BOOST_CHECK_EQUAL(db.get_string(), "123456789012345");
    }

    BOOST_AUTO_TEST_CASE(test_basis_database_uint64_constructor)
    {
        const std::string field_name{ "uid" };
        constexpr auto value = 987654321098765u;
        const celeritas::basis_database db{ field_name, value };

        BOOST_CHECK_EQUAL(db.get_field_name(), field_name);
        BOOST_CHECK(db.get_data_type() == celeritas::database_data_type::int64_type);
        BOOST_CHECK_EQUAL(db.get_value<celeritas::database_data_type::int64_type>(), value);
    }

    BOOST_AUTO_TEST_CASE(test_basis_database_bool_constructor)
    {
        const std::string field_name{ "is_active" };
        constexpr auto value = true;
        const celeritas::basis_database db{ field_name, value };

        BOOST_CHECK_EQUAL(db.get_field_name(), field_name);
        BOOST_CHECK(db.get_data_type() == celeritas::database_data_type::bool_type);
        BOOST_CHECK_EQUAL(db.get_value<celeritas::database_data_type::bool_type>(), value);
        BOOST_CHECK_EQUAL(db.get_string(), "1");
    }

    BOOST_AUTO_TEST_CASE(test_basis_database_double_constructor)
    {
        const std::string field_name{ "score" };
        constexpr auto value = 99.9;
        const celeritas::basis_database db{ field_name, value };

        BOOST_CHECK_EQUAL(db.get_field_name(), field_name);
        BOOST_CHECK(db.get_data_type() == celeritas::database_data_type::double_type);
        BOOST_CHECK_CLOSE(db.get_value<celeritas::database_data_type::double_type>(), value, 0.001);
        BOOST_CHECK_EQUAL(db.get_string(), "99.9");
    }

    BOOST_AUTO_TEST_CASE(test_basis_database_byte_array_constructor)
    {
        const std::string field_name{ "data" };
        const celeritas::basis_database::byte_array value{ 'a', 'b', 'c' };
        const celeritas::basis_database db{ field_name, value };

        BOOST_CHECK_EQUAL(db.get_field_name(), field_name);
        BOOST_CHECK(db.get_data_type() == celeritas::database_data_type::byte_array_type);
        const auto& arr = db.get_value<celeritas::database_data_type::byte_array_type>();
        BOOST_CHECK_EQUAL_COLLECTIONS(arr.begin(), arr.end(), value.begin(), value.end());
    }

    BOOST_AUTO_TEST_CASE(test_basis_database_string_array_constructor)
    {
        const std::string field_name{ "tags" };
        const celeritas::basis_database::string_array value{ "tag1", "tag2" };
        const celeritas::basis_database db{ field_name, value };

        BOOST_CHECK_EQUAL(db.get_field_name(), field_name);
        BOOST_CHECK(db.get_data_type() == celeritas::database_data_type::string_array_type);
        const auto& arr = db.get_value<celeritas::database_data_type::string_array_type>();
        BOOST_CHECK_EQUAL_COLLECTIONS(arr.begin(), arr.end(), value.begin(), value.end());
        BOOST_CHECK_EQUAL(db.get_array_string_value<celeritas::database_data_type::string_array_type>(), "'tag1','tag2'");
    }

    BOOST_AUTO_TEST_CASE(test_basis_database_int32_array_constructor)
    {
        const std::string field_name{ "numbers" };
        const celeritas::basis_database::int32_array value{ 1, 2, 3 };
        const celeritas::basis_database db{ field_name, value };

        BOOST_CHECK_EQUAL(db.get_field_name(), field_name);
        BOOST_CHECK(db.get_data_type() == celeritas::database_data_type::int32_array_type);
        const auto& arr = db.get_value<celeritas::database_data_type::int32_array_type>();
        BOOST_CHECK_EQUAL_COLLECTIONS(arr.begin(), arr.end(), value.begin(), value.end());
        BOOST_CHECK_EQUAL(db.get_array_string_value<celeritas::database_data_type::int32_array_type>(), "1,2,3");
    }

    BOOST_AUTO_TEST_CASE(test_basis_database_int64_array_constructor)
    {
        const std::string field_name{ "big_numbers" };
        const celeritas::basis_database::int64_array value{ 100, 200, 300 };
        const celeritas::basis_database db{ field_name, value };

        BOOST_CHECK_EQUAL(db.get_field_name(), field_name);
        BOOST_CHECK(db.get_data_type() == celeritas::database_data_type::int64_array_type);
        const auto& arr = db.get_value<celeritas::database_data_type::int64_array_type>();
        BOOST_CHECK_EQUAL_COLLECTIONS(arr.begin(), arr.end(), value.begin(), value.end());
        BOOST_CHECK_EQUAL(db.get_array_string_value<celeritas::database_data_type::int64_array_type>(), "100,200,300");
    }

    BOOST_AUTO_TEST_CASE(test_basis_database_double_array_constructor)
    {
        const std::string field_name{ "scores" };
        const celeritas::basis_database::double_array value{ 99.9, 88.8, 77.7 };
        const celeritas::basis_database db{ field_name, value };

        BOOST_CHECK_EQUAL(db.get_field_name(), field_name);
        BOOST_CHECK(db.get_data_type() == celeritas::database_data_type::double_array_type);
        const auto& arr = db.get_value<celeritas::database_data_type::double_array_type>();
        BOOST_CHECK_EQUAL_COLLECTIONS(arr.begin(), arr.end(), value.begin(), value.end());
    }

    BOOST_AUTO_TEST_CASE(test_basis_database_document_constructor)
    {
        const std::string field_name{ "doc" };
        const celeritas::basis_database::document_type value{
            { "field1", "value1" },
            { "field2", 123 }
        };
        const celeritas::basis_database db{ field_name, value };

        BOOST_CHECK_EQUAL(db.get_field_name(), field_name);
        BOOST_CHECK(db.get_data_type() == celeritas::database_data_type::document_type);
        BOOST_CHECK_EQUAL(db.get_string(), "{\"field1\":\"value1\",\"field2\":123}");
    }

    BOOST_AUTO_TEST_CASE(test_basis_database_document_array_constructor)
    {
        const std::string field_name{ "docs" };
        const celeritas::basis_database::document_array value{
            {
                { "field1", "value1" }
            },
            {
                { "field2", 123 }
            }
        };
        const celeritas::basis_database db{ field_name, value };

        BOOST_CHECK_EQUAL(db.get_field_name(), field_name);
        BOOST_CHECK(db.get_data_type() == celeritas::database_data_type::document_array_type);
        BOOST_CHECK_EQUAL(db.get_string(), "[{\"field1\":\"value1\"},{\"field2\":123}]");
    }

    BOOST_AUTO_TEST_CASE(test_basis_database_get_sql_field_string)
    {
        const celeritas::basis_database db{ "name", "test" };
        BOOST_CHECK_EQUAL(db.get_sql_field_string(), "name='test'");
    }

    BOOST_AUTO_TEST_CASE(test_basis_database_operators)
    {
        const celeritas::basis_database db1{ "name", "test" };
        const celeritas::basis_database db2{ "name", "test" };
        const celeritas::basis_database db3{ "name", "another" };
        const celeritas::basis_database db4{ "age", 30 };

        BOOST_CHECK(db1 == db2);
        BOOST_CHECK(db1 != db3);
        BOOST_CHECK(db1 != db4);
    }

BOOST_AUTO_TEST_SUITE_END()
