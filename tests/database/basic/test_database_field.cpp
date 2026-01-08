#include "database/basic/database_data_type.h"
#include "database/basic/database_field.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(database_field_suite)

    BOOST_AUTO_TEST_CASE(test_database_field_constructor)
    {
        constexpr auto field_name{ "test_field" };
        constexpr auto data_type = celeritas::database_data_type::int32_type;
        constexpr auto index_type = celeritas::database_index_type::key;

        constexpr celeritas::database_field field{ field_name, data_type, index_type };

        BOOST_CHECK_EQUAL(field.get_field_name(), field_name);
        BOOST_CHECK(field.get_data_type() == data_type);
        BOOST_CHECK(field.get_index_type() == index_type);
    }

    BOOST_AUTO_TEST_CASE(test_database_field_default_index_type)
    {
        constexpr auto field_name{ "test_field_default" };
        constexpr auto data_type = celeritas::database_data_type::string_type;

        constexpr celeritas::database_field field{ field_name, data_type };

        BOOST_CHECK_EQUAL(field.get_field_name(), field_name);
        BOOST_CHECK(field.get_data_type() == data_type);
        BOOST_CHECK(field.get_index_type() == celeritas::database_index_type::null);
    }

    BOOST_AUTO_TEST_CASE(test_database_field_empty_name)
    {
        constexpr auto field_name{ "" };
        constexpr auto data_type = celeritas::database_data_type::int64_type;
        constexpr auto index_type = celeritas::database_index_type::key;

        constexpr celeritas::database_field field{ field_name, data_type, index_type };

        BOOST_CHECK_EQUAL(field.get_field_name(), field_name);
        BOOST_CHECK(field.get_data_type() == data_type);
        BOOST_CHECK(field.get_index_type() == index_type);
    }

BOOST_AUTO_TEST_SUITE_END()
