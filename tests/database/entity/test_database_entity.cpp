#include "config/basic/database_type.h"
#include "database/basic/database_change_type.h"
#include "database/basic/database_entity_change.h"
#include "database/entity/database_entity.h"
#include "fixture/database_entity_fixture.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(database_entity_suite)

    BOOST_FIXTURE_TEST_CASE(test_database_entity_constructor, celeritas::database_entity_fixture)
    {
        const celeritas::database_entity entity{ get_db_type(), get_db_name(), get_key() };
        BOOST_CHECK(!entity.is_modify());
        BOOST_CHECK(!entity.is_must_save());
    }

    BOOST_FIXTURE_TEST_CASE(test_database_entity_get_modify, celeritas::database_entity_fixture)
    {
        const celeritas::database_entity entity{ get_db_type(), get_db_name(), get_key() };
        const auto modify = entity.get_modify();
        BOOST_CHECK(modify->get_change_type() == celeritas::database_change_type::insert_type);
        BOOST_CHECK(modify->get_key() == get_key());
    }

    BOOST_FIXTURE_TEST_CASE(test_database_entity_get_delete, celeritas::database_entity_fixture)
    {
        const celeritas::database_entity entity{ get_db_type(), get_db_name(), get_key() };
        const auto database_entity_change = entity.get_delete();
        BOOST_CHECK(database_entity_change->get_change_type() == celeritas::database_change_type::delete_type);
        BOOST_CHECK(database_entity_change->get_key() == get_key());
    }

    BOOST_FIXTURE_TEST_CASE(test_database_entity_clear_modify, celeritas::database_entity_fixture)
    {
        celeritas::database_entity entity{ get_db_type(), get_db_name(), get_key() };
        entity.clear_modify();
        BOOST_CHECK(!entity.is_modify());
    }

BOOST_AUTO_TEST_SUITE_END()
