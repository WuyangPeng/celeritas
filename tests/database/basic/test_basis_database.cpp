#include "database/basic/basis_database.tpp"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(basis_database_suite)

    BOOST_AUTO_TEST_CASE(test_basis_database_null_constructor)
    {
        const std::string field_name{ "null_field" };
        const celeritas::basis_database db{ field_name };

        BOOST_CHECK_EQUAL(db.get_field_name(), field_name);
        BOOST_CHECK(db.get_data_type() == celeritas::database_data_type::null_type);
        BOOST_CHECK(std::holds_alternative<std::monostate>(db.get_variant_value()));
        BOOST_CHECK_EQUAL(db.get_string(), "");
    }

    BOOST_AUTO_TEST_CASE(test_basis_database_string_constructor)
    {
        const std::string field_name{ "name" };
        const std::string value{ "test_name" };
        const celeritas::basis_database db{ field_name, value };

        BOOST_CHECK_EQUAL(db.get_field_name(), field_name);
        BOOST_CHECK(db.get_data_type() == celeritas::database_data_type::string_type);
        BOOST_CHECK_EQUAL(db.get_value<celeritas::database_data_type::string_type>(), value);
        BOOST_CHECK_EQUAL(db.get_string(), "test_name");
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

    BOOST_AUTO_TEST_CASE(test_basis_database_bool_true_constructor)
    {
        const std::string field_name{ "is_active" };
        const celeritas::basis_database db{ field_name, true };

        BOOST_CHECK_EQUAL(db.get_field_name(), field_name);
        BOOST_CHECK(db.get_data_type() == celeritas::database_data_type::bool_type);
        BOOST_CHECK_EQUAL(db.get_value<celeritas::database_data_type::bool_type>(), true);
        BOOST_CHECK_EQUAL(db.get_string(), "true");
    }

    BOOST_AUTO_TEST_CASE(test_basis_database_bool_false_constructor)
    {
        const std::string field_name{ "is_active" };
        const celeritas::basis_database db{ field_name, false };

        BOOST_CHECK_EQUAL(db.get_field_name(), field_name);
        BOOST_CHECK(db.get_data_type() == celeritas::database_data_type::bool_type);
        BOOST_CHECK_EQUAL(db.get_value<celeritas::database_data_type::bool_type>(), false);
        BOOST_CHECK_EQUAL(db.get_string(), "false");
    }

    BOOST_AUTO_TEST_CASE(test_basis_database_double_constructor)
    {
        const std::string field_name{ "score" };
        constexpr auto value = 99.9;
        const celeritas::basis_database db{ field_name, value };

        BOOST_CHECK_EQUAL(db.get_field_name(), field_name);
        BOOST_CHECK(db.get_data_type() == celeritas::database_data_type::double_type);
        BOOST_CHECK_CLOSE(db.get_value<celeritas::database_data_type::double_type>(), value, 0.001);
        BOOST_CHECK_EQUAL(db.get_string(), std::to_string(value));
    }

    BOOST_AUTO_TEST_CASE(test_basis_database_byte_array_constructor)
    {
        const std::string field_name{ "data" };
        const celeritas::basis_database::byte_array value{ 'a', 'b', 'c' };
        const celeritas::basis_database db{ field_name, value };

        BOOST_CHECK_EQUAL(db.get_field_name(), field_name);
        BOOST_CHECK(db.get_data_type() == celeritas::database_data_type::byte_array_type);
        const auto& value_from_db = db.get_value<celeritas::database_data_type::byte_array_type>();
        BOOST_CHECK_EQUAL_COLLECTIONS(value_from_db.begin(), value_from_db.end(), value.begin(), value.end());
        BOOST_CHECK_EQUAL(db.get_string(), "abc");
    }

    BOOST_AUTO_TEST_CASE(test_basis_database_string_array_constructor)
    {
        const std::string field_name{ "tags" };
        const celeritas::basis_database::string_array value{ "tag1", "tag2" };
        const celeritas::basis_database db{ field_name, value };

        BOOST_CHECK_EQUAL(db.get_field_name(), field_name);
        BOOST_CHECK(db.get_data_type() == celeritas::database_data_type::string_array_type);
        const auto& value_from_db = db.get_value<celeritas::database_data_type::string_array_type>();
        BOOST_CHECK_EQUAL_COLLECTIONS(value_from_db.begin(), value_from_db.end(), value.begin(), value.end());
        BOOST_CHECK_EQUAL(db.get_string(), "[\"tag1\",\"tag2\"]");
    }

    BOOST_AUTO_TEST_CASE(test_basis_database_int32_array_constructor)
    {
        const std::string field_name{ "numbers" };
        const celeritas::basis_database::int32_array value{ 1, 2, 3 };
        const celeritas::basis_database db{ field_name, value };

        BOOST_CHECK_EQUAL(db.get_field_name(), field_name);
        BOOST_CHECK(db.get_data_type() == celeritas::database_data_type::int32_array_type);
        const auto& value_from_db = db.get_value<celeritas::database_data_type::int32_array_type>();
        BOOST_CHECK_EQUAL_COLLECTIONS(value_from_db.begin(), value_from_db.end(), value.begin(), value.end());
        BOOST_CHECK_EQUAL(db.get_string(), "[1,2,3]");
    }

    BOOST_AUTO_TEST_CASE(test_basis_database_int64_array_constructor)
    {
        const std::string field_name{ "big_numbers" };
        const celeritas::basis_database::int64_array value{ 100, 200, 300 };
        const celeritas::basis_database db{ field_name, value };

        BOOST_CHECK_EQUAL(db.get_field_name(), field_name);
        BOOST_CHECK(db.get_data_type() == celeritas::database_data_type::int64_array_type);
        const auto& value_from_db = db.get_value<celeritas::database_data_type::int64_array_type>();
        BOOST_CHECK_EQUAL_COLLECTIONS(value_from_db.begin(), value_from_db.end(), value.begin(), value.end());
        BOOST_CHECK_EQUAL(db.get_string(), "[100,200,300]");
    }

    BOOST_AUTO_TEST_CASE(test_basis_database_double_array_constructor)
    {
        const std::string field_name{ "scores" };
        const celeritas::basis_database::double_array value{ 99.9, 88.8, 77.7 };
        const celeritas::basis_database db{ field_name, value };

        BOOST_CHECK_EQUAL(db.get_field_name(), field_name);
        BOOST_CHECK(db.get_data_type() == celeritas::database_data_type::double_array_type);
        const auto& value_from_db = db.get_value<celeritas::database_data_type::double_array_type>();
        BOOST_CHECK_EQUAL_COLLECTIONS(value_from_db.begin(), value_from_db.end(), value.begin(), value.end());
        BOOST_CHECK_EQUAL(db.get_string(), "[99.9,88.8,77.7]");
    }

    BOOST_AUTO_TEST_CASE(test_basis_database_document_non_empty_constructor)
    {
        const std::string field_name{ "doc" };
        const celeritas::basis_database::document_type value{ { "field1", "value1" },
                                                              { "field2", 123 } };
        const celeritas::basis_database db{ field_name, value };

        BOOST_CHECK_EQUAL(db.get_field_name(), field_name);
        BOOST_CHECK(db.get_data_type() == celeritas::database_data_type::document_type);
        BOOST_CHECK_EQUAL(db.get_string(), "{\"field1\":\"value1\",\"field2\":123}");
    }

    BOOST_AUTO_TEST_CASE(test_basis_database_document_empty_constructor)
    {
        const std::string field_name{ "doc" };
        const celeritas::basis_database db{ field_name, celeritas::basis_database::document_type{} };

        BOOST_CHECK_EQUAL(db.get_field_name(), field_name);
        BOOST_CHECK(db.get_data_type() == celeritas::database_data_type::document_type);
        BOOST_CHECK_EQUAL(db.get_string(), "{}");
    }

    BOOST_AUTO_TEST_CASE(test_basis_database_document_array_non_empty_constructor)
    {
        const std::string field_name{ "docs" };
        const celeritas::basis_database::document_array value{ { { "field1", "value1" } },
                                                               { { "field2", 123 } } };
        const celeritas::basis_database db{ field_name, value };

        BOOST_CHECK_EQUAL(db.get_field_name(), field_name);
        BOOST_CHECK(db.get_data_type() == celeritas::database_data_type::document_array_type);
        BOOST_CHECK_EQUAL(db.get_string(), "[{\"field1\":\"value1\"},{\"field2\":123}]");
    }

    BOOST_AUTO_TEST_CASE(test_basis_database_document_array_empty_constructor)
    {
        const std::string field_name{ "docs" };
        const celeritas::basis_database db{ field_name, celeritas::basis_database::document_array{} };

        BOOST_CHECK_EQUAL(db.get_field_name(), field_name);
        BOOST_CHECK(db.get_data_type() == celeritas::database_data_type::document_array_type);
        BOOST_CHECK_EQUAL(db.get_string(), "[]");
    }

    BOOST_AUTO_TEST_CASE(test_basis_database_has_value_string)
    {
        const celeritas::basis_database db_string{ "name", "test" };
        BOOST_CHECK(db_string.has_value<celeritas::database_data_type::string_type>());
        BOOST_CHECK(!db_string.has_value<celeritas::database_data_type::int32_type>());
    }

    BOOST_AUTO_TEST_CASE(test_basis_database_has_value_int)
    {
        const celeritas::basis_database db_int{ "age", 30 };
        BOOST_CHECK(db_int.has_value<celeritas::database_data_type::int32_type>());
        BOOST_CHECK(!db_int.has_value<celeritas::database_data_type::string_type>());
    }

    BOOST_AUTO_TEST_CASE(test_basis_database_has_value_null)
    {
        const celeritas::basis_database db_null{ "null_field" };
        BOOST_CHECK(!db_null.has_value<celeritas::database_data_type::string_type>());
    }

    BOOST_AUTO_TEST_CASE(test_get_sql_value_string)
    {
        const celeritas::basis_database db_string{ "name", "test" };
        BOOST_CHECK_EQUAL(db_string.get_sql_value_string(), "'test'");

        const celeritas::basis_database db_int{ "age", 30 };
        BOOST_CHECK_EQUAL(db_int.get_sql_value_string(), "30");
    }

    BOOST_AUTO_TEST_CASE(test_get_quotation_mark_string)
    {
        const celeritas::basis_database db_string{ "name", "test" };
        BOOST_CHECK_EQUAL(db_string.get_quotation_mark_string(), "\"test\"");

        const celeritas::basis_database db_int{ "age", 30 };
        BOOST_CHECK_EQUAL(db_int.get_quotation_mark_string(), "30");
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
