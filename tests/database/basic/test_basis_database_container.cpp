#include "database/basic/basis_database_container.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(basis_database_container_suite)

    BOOST_AUTO_TEST_CASE(test_basis_database_container_default_constructor)
    {
        const celeritas::basis_database_container container{};
        BOOST_CHECK_EQUAL(container.get_size(), 0);
        BOOST_CHECK(container.begin() == container.end());
    }

    BOOST_AUTO_TEST_CASE(test_basis_database_container_single_element_constructor)
    {
        const celeritas::basis_database db{ "name", "test" };
        const celeritas::basis_database_container container{ db };

        BOOST_CHECK_EQUAL(container.get_size(), 1);
        BOOST_CHECK(container.begin() != container.end());
        BOOST_CHECK(*container.begin() == db);
    }

    BOOST_AUTO_TEST_CASE(test_basis_database_container_vector_constructor)
    {
        const celeritas::basis_database db1{ "name", "test" };
        const celeritas::basis_database db2{ "age", 30 };
        const std::vector vec{ db1, db2 };
        const celeritas::basis_database_container container{ vec };

        BOOST_CHECK_EQUAL(container.get_size(), 2);
        auto iter = container.begin();
        BOOST_CHECK(*iter == db1);
        ++iter;
        BOOST_CHECK(*iter == db2);
    }

    BOOST_AUTO_TEST_CASE(test_basis_database_container_modify)
    {
        const celeritas::basis_database db1{ "name", "test" };
        celeritas::basis_database_container container{ db1 };

        const celeritas::basis_database db2{ "name", "modified" };
        container.modify(db2);

        BOOST_CHECK_EQUAL(container.get_size(), 1);
        BOOST_CHECK(*container.begin() == db2);

        const celeritas::basis_database db3{ "age", 30 };
        container.modify(db3);
        BOOST_CHECK_EQUAL(container.get_size(), 2);
    }

    BOOST_AUTO_TEST_CASE(test_basis_database_container_set)
    {
        const celeritas::basis_database db1{ "name", "test" };
        celeritas::basis_database_container container{ db1 };

        const celeritas::basis_database db2{ "age", 30 };
        const std::vector vec{ db2 };
        container.set(vec);

        BOOST_CHECK_EQUAL(container.get_size(), 1);
        BOOST_CHECK(*container.begin() == db2);
    }

    BOOST_AUTO_TEST_CASE(test_basis_database_container_clear)
    {
        const celeritas::basis_database db1{ "name", "test" };
        celeritas::basis_database_container container{ db1 };

        container.clear();
        BOOST_CHECK_EQUAL(container.get_size(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_basis_database_container_get_variant_value)
    {
        const celeritas::basis_database db1{ "name", "test" };
        const celeritas::basis_database_container container{ db1 };

        const auto& value = container.get_variant_value("name");
        BOOST_CHECK(std::holds_alternative<std::string>(value));
        BOOST_CHECK_EQUAL(std::get<std::string>(value), "test");

        const auto& value_missing = container.get_variant_value("missing");
        BOOST_CHECK(std::holds_alternative<std::monostate>(value_missing));
    }

BOOST_AUTO_TEST_SUITE_END()
