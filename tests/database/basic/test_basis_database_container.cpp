#include "common/core/celeritas_error.h"
#include "database/basic/basis_database_container.h"

#include <boost/test/unit_test.hpp>

namespace
{
    template <celeritas::database_data_type Type>
    const auto& get_value(const celeritas::basis_database_container& container, const std::string& field_name)
    {
        const auto& value = container.get_variant_value(field_name);
        return std::get<typename celeritas::database_data_type_traits<Type>::type>(value);
    }
}

BOOST_AUTO_TEST_SUITE(basis_database_container_suite)

    BOOST_AUTO_TEST_CASE(test_default_constructor)
    {
        const celeritas::basis_database_container container{};
        BOOST_CHECK_EQUAL(container.get_size(), 0);
        BOOST_CHECK(container.begin() == container.end());
    }

    BOOST_AUTO_TEST_CASE(test_single_element_constructor)
    {
        const celeritas::basis_database db{ "name", "test" };
        const celeritas::basis_database_container container{ db };

        BOOST_CHECK_EQUAL(container.get_size(), 1);
        BOOST_CHECK(container.begin() != container.end());
        BOOST_CHECK(*container.begin() == db);
    }

    BOOST_AUTO_TEST_CASE(test_vector_constructor)
    {
        const celeritas::basis_database db1{ "name", "test" };
        const celeritas::basis_database db2{ "age", 30 };
        const std::vector db_container{ db1, db2 };
        const celeritas::basis_database_container container{ db_container };

        BOOST_CHECK_EQUAL(container.get_size(), 2);
        auto iter = container.begin();
        BOOST_CHECK(*iter == db1);
        ++iter;
        BOOST_CHECK(*iter == db2);
    }

    BOOST_AUTO_TEST_CASE(test_vector_constructor_with_duplicates_throws)
    {
        const celeritas::basis_database_container::object_container dbs_with_duplicates{ { "name", "test1" },
                                                                                         { "age", 30 },
                                                                                         { "name", "test2" } };

        BOOST_CHECK_THROW(celeritas::basis_database_container{ dbs_with_duplicates }, celeritas::celeritas_error);
    }

    BOOST_AUTO_TEST_CASE(test_modify_existing_element)
    {
        celeritas::basis_database_container container{ celeritas::basis_database{ "name", "test" } };
        const celeritas::basis_database db_modified{ "name", "modified" };
        container.modify(db_modified);

        BOOST_CHECK_EQUAL(container.get_size(), 1);
        BOOST_CHECK_EQUAL(get_value<celeritas::database_data_type::string_type>(container, "name"), "modified");
    }

    BOOST_AUTO_TEST_CASE(test_modify_add_new_element)
    {
        celeritas::basis_database_container container{ celeritas::basis_database{ "name", "test" } };
        const celeritas::basis_database db_new{ "age", 30 };
        container.modify(db_new);

        BOOST_CHECK_EQUAL(container.get_size(), 2);
        BOOST_CHECK_EQUAL(get_value<celeritas::database_data_type::int32_type>(container, "age"), 30);
    }

    BOOST_AUTO_TEST_CASE(test_clear)
    {
        celeritas::basis_database_container container{ celeritas::basis_database{ "name", "test" } };
        container.clear();
        BOOST_CHECK_EQUAL(container.get_size(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_get_variant_value_string)
    {
        const celeritas::basis_database_container container{ celeritas::basis_database_container::object_container{ { "name", "test" } } };
        const auto& value = container.get_variant_value("name");
        BOOST_CHECK(std::holds_alternative<std::string>(value));
        BOOST_CHECK_EQUAL(std::get<std::string>(value), "test");
    }

    BOOST_AUTO_TEST_CASE(test_get_variant_value_int)
    {
        const celeritas::basis_database_container container{ celeritas::basis_database_container::object_container{ { "age", 30 } } };
        const auto& value = container.get_variant_value("age");

        BOOST_CHECK(std::holds_alternative<int32_t>(value));
        BOOST_CHECK_EQUAL(std::get<int32_t>(value), 30);
    }

    BOOST_AUTO_TEST_CASE(test_get_variant_value_missing)
    {
        const celeritas::basis_database_container container{ celeritas::basis_database_container::object_container{} };
        const auto& value = container.get_variant_value("missing");

        BOOST_CHECK(std::holds_alternative<std::monostate>(value));
    }

    BOOST_AUTO_TEST_CASE(test_get_value)
    {
        const celeritas::basis_database_container container{ celeritas::basis_database_container::object_container{ { "name", "test" }, { "age", 30 } } };

        BOOST_CHECK_EQUAL(get_value<celeritas::database_data_type::string_type>(container, "name"), "test");
        BOOST_CHECK_EQUAL(get_value<celeritas::database_data_type::int32_type>(container, "age"), 30);
        BOOST_CHECK_THROW(get_value<celeritas::database_data_type::string_type>(container, "missing"), std::bad_variant_access);
    }

    BOOST_AUTO_TEST_CASE(test_equality_identical_containers)
    {
        const celeritas::basis_database_container container1{ celeritas::basis_database_container::object_container{ { "name", "test" }, { "age", 30 } } };
        const celeritas::basis_database_container container2{ celeritas::basis_database_container::object_container{ { "name", "test" }, { "age", 30 } } };

        BOOST_CHECK(container1 == container2);
        BOOST_CHECK(!(container1 != container2));
    }

    BOOST_AUTO_TEST_CASE(test_equality_different_order)
    {
        const celeritas::basis_database_container container1{ celeritas::basis_database_container::object_container{ { "name", "test" }, { "age", 30 } } };
        const celeritas::basis_database_container container2{ celeritas::basis_database_container::object_container{ { "age", 30 }, { "name", "test" } } };

        BOOST_CHECK(container1 != container2);
        BOOST_CHECK(!(container1 == container2));
    }

    BOOST_AUTO_TEST_CASE(test_inequality_different_values)
    {
        const celeritas::basis_database_container container1{ celeritas::basis_database_container::object_container{ { "name", "test" }, { "age", 30 } } };
        const celeritas::basis_database_container container2{ celeritas::basis_database_container::object_container{ { "name", "test" }, { "age", 31 } } };

        BOOST_CHECK(container1 != container2);
        BOOST_CHECK(!(container1 == container2));
    }

    BOOST_AUTO_TEST_CASE(test_inequality_different_sizes)
    {
        const celeritas::basis_database_container container1{ celeritas::basis_database_container::object_container{ { "name", "test" }, { "age", 30 } } };
        const celeritas::basis_database_container container2{ celeritas::basis_database_container::object_container{ { "name", "test" } } };

        BOOST_CHECK(container1 != container2);
        BOOST_CHECK(!(container1 == container2));
    }

    BOOST_AUTO_TEST_CASE(test_inequality_different_fields)
    {
        const celeritas::basis_database_container container1{ celeritas::basis_database_container::object_container{ { "name", "test" }, { "age", 30 } } };
        const celeritas::basis_database_container container2{ celeritas::basis_database_container::object_container{ { "name", "test" }, { "city", "ny" } } };

        BOOST_CHECK(container1 != container2);
        BOOST_CHECK(!(container1 == container2));
    }

    BOOST_AUTO_TEST_CASE(test_equality_empty_containers)
    {
        const celeritas::basis_database_container empty_container1{};
        const celeritas::basis_database_container empty_container2{};

        BOOST_CHECK(empty_container1 == empty_container2);
        BOOST_CHECK(!(empty_container1 != empty_container2));
    }

    BOOST_AUTO_TEST_CASE(test_inequality_with_empty_container)
    {
        const celeritas::basis_database_container container1{ celeritas::basis_database_container::object_container{ { "name", "test" } } };
        const celeritas::basis_database_container empty_container{};

        BOOST_CHECK(container1 != empty_container);
        BOOST_CHECK(!(container1 == empty_container));
    }

    BOOST_AUTO_TEST_CASE(test_get_basis_database)
    {
        const celeritas::basis_database db1{ "name", "test" };
        const celeritas::basis_database db2{ "age", 30 };
        const celeritas::basis_database_container container{ celeritas::basis_database_container::object_container{ db1, db2 } };

        const auto& found_db1 = container.get_basis_database("name");
        BOOST_CHECK(found_db1 == db1);

        const auto& found_db2 = container.get_basis_database("age");
        BOOST_CHECK(found_db2 == db2);

        BOOST_CHECK_THROW(std::ignore = container.get_basis_database("missing"), celeritas::celeritas_error);
    }

BOOST_AUTO_TEST_SUITE_END()
