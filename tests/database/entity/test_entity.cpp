#include "database/basic/database_data_type.h"
#include "database/basic/database_data_type_traits.h"
#include "database/entity/entity.tpp"

#include <boost/test/unit_test.hpp>

#include <string>
#include <vector>

namespace
{
    constexpr std::string_view field_name = "test_field";
    constexpr std::string_view string_field_name = "string_field";
    constexpr std::string_view indexed_field_name = "indexed_field";
    constexpr std::string_view array_field_name = "array_field";

    constexpr auto data_type = celeritas::database_data_type::int32_type;
    constexpr auto string_data_type = celeritas::database_data_type::string_type;
    constexpr auto indexed_data_type = celeritas::database_data_type::int64_type;
    constexpr auto index_type = celeritas::database_index_type::unique_index;
    constexpr auto array_data_type = celeritas::database_data_type::int32_array_type;
}

BOOST_AUTO_TEST_SUITE(entity_suite)

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

    BOOST_AUTO_TEST_CASE(test_entity_get_database_field)
    {
        using entity_type = celeritas::entity<field_name, data_type>;
        const auto database_field = entity_type::get_database_field();

        BOOST_CHECK_EQUAL(database_field.get_field_name(), "test_field");
        BOOST_CHECK(database_field.get_data_type() == data_type);
        BOOST_CHECK(database_field.get_index_type() == celeritas::database_index_type::null);
    }

    BOOST_AUTO_TEST_CASE(test_entity_array_get_value)
    {
        const std::vector initial_value{ 1, 2, 3 };
        celeritas::entity<array_field_name, array_data_type> array_entity{ initial_value };

        BOOST_CHECK(array_entity.get_value() == initial_value);
    }

    BOOST_AUTO_TEST_CASE(test_entity_array_add_value)
    {
        const std::vector initial_value{ 1, 2, 3 };
        celeritas::entity<array_field_name, array_data_type> array_entity{ initial_value };

        array_entity.add_value(4);
        const std::vector expected_after_add{ 1, 2, 3, 4 };
        BOOST_CHECK(array_entity.get_value() == expected_after_add);
    }

    BOOST_AUTO_TEST_CASE(test_entity_array_set_value_by_index)
    {
        const std::vector initial_value{ 1, 2, 3 };
        celeritas::entity<array_field_name, array_data_type> array_entity{ initial_value };

        const auto changed = array_entity.set_value(0, 10);
        BOOST_CHECK(changed);
        const std::vector expected_after_set = { 10, 2, 3 };
        BOOST_CHECK(array_entity.get_value() == expected_after_set);
    }

    BOOST_AUTO_TEST_CASE(test_entity_array_set_value_by_index_no_change)
    {
        const std::vector initial_value{ 1, 2, 3 };
        celeritas::entity<array_field_name, array_data_type> array_entity{ initial_value };

        const auto changed = array_entity.set_value(0, 1);
        BOOST_CHECK(!changed);
    }

    BOOST_AUTO_TEST_CASE(test_entity_array_remove_value)
    {
        const std::vector initial_value{ 1, 2, 3 };
        celeritas::entity<array_field_name, array_data_type> array_entity{ initial_value };

        array_entity.remove_value(0);
        std::vector expected_after_remove = { 2, 3 };
        BOOST_CHECK(array_entity.get_value() == expected_after_remove);
    }

    BOOST_AUTO_TEST_CASE(test_entity_array_remove_value_out_of_bounds)
    {
        const std::vector initial_value = { 1, 2, 3 };
        celeritas::entity<array_field_name, array_data_type> array_entity{ initial_value };

        array_entity.remove_value(100);
        BOOST_CHECK(array_entity.get_value() == initial_value);
    }

    BOOST_AUTO_TEST_CASE(test_entity_string_operations)
    {
        celeritas::entity<string_field_name, string_data_type> string_entity{ "hello" };

        BOOST_CHECK_EQUAL(string_entity.get_value(), "hello");

        string_entity.set_value("world");
        BOOST_CHECK_EQUAL(string_entity.get_value(), "world");

        string_entity.modify_value("!");
        BOOST_CHECK_EQUAL(string_entity.get_value(), "world!");
    }

    BOOST_AUTO_TEST_CASE(test_entity_with_index)
    {
        using entity_type = celeritas::entity<indexed_field_name, indexed_data_type, index_type>;
        entity_type indexed_entity{ 100 };

        BOOST_CHECK(indexed_entity.get_index_type() == index_type);

        const auto database_field = entity_type::get_database_field();
        BOOST_CHECK(database_field.get_index_type() == index_type);
    }

BOOST_AUTO_TEST_SUITE_END()
