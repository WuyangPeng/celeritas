#include "database/basic/database_data_type.h"
#include "database/basic/database_entity_change.h"
#include "database/entity/entity.tpp"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(entity_suite)

    constexpr std::string_view field_name = "test_field";
    constexpr auto data_type = celeritas::database_data_type::int32_type;

    BOOST_AUTO_TEST_CASE(test_entity_constructor)
    {
        const celeritas::entity<field_name, data_type> test_entity{ 10 };

        BOOST_CHECK_EQUAL(test_entity.get_value(), 10);
    }

    BOOST_AUTO_TEST_CASE(test_entity_getters)
    {
        celeritas::entity<field_name, data_type> test_entity{ 10 };
        BOOST_CHECK_EQUAL(test_entity.get_field_name(), "test_field");
        BOOST_CHECK(test_entity.get_data_type() == data_type);
        BOOST_CHECK(test_entity.get_index_type() == celeritas::database_index_type::null);
    }

    BOOST_AUTO_TEST_CASE(test_entity_set_value)
    {
        celeritas::entity<field_name, data_type> test_entity{ 10 };

        test_entity.set_value(20);
        BOOST_CHECK_EQUAL(test_entity.get_value(), 20);
    }

    BOOST_AUTO_TEST_CASE(test_entity_modify_value)
    {
        celeritas::entity<field_name, data_type> test_entity{ 10 };

        test_entity.modify_value(30);
        BOOST_CHECK_EQUAL(test_entity.get_value(), 40);
    }

BOOST_AUTO_TEST_SUITE_END()
