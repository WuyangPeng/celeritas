#include "config/basic/database_type.h"
#include "database/basic/database_entity_change.h"
#include "database/database_constant.h"
#include "database/basic/database_change_type.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(database_entity_change_suite)

    BOOST_AUTO_TEST_CASE(test_database_entity_change_constructor_with_key)
    {
        constexpr auto db_type = celeritas::database_type::mysql;
        const std::string db_name{ "test_db" };
        constexpr auto change_type = celeritas::database_change_type::insert_type;
        const auto key = std::make_shared<celeritas::basis_database_container>(celeritas::basis_database{ "id", 1 });

        const celeritas::database_entity_change change{ db_type, db_name, change_type, key };

        BOOST_CHECK(change.get_database_type() == db_type);
        BOOST_CHECK_EQUAL(change.get_database_name(), db_name);
        BOOST_CHECK(change.get_change_type() == change_type);
        BOOST_CHECK(change.get_key() == key);
        BOOST_CHECK(change.get_database() != nullptr);
    }

    BOOST_AUTO_TEST_CASE(test_database_entity_change_constructor_without_key)
    {
        constexpr auto db_type = celeritas::database_type::redis;
        const std::string db_name{ "test_redis" };
        constexpr auto change_type = celeritas::database_change_type::update_type;

        const celeritas::database_entity_change change{ db_type, db_name, change_type };

        BOOST_CHECK(change.get_database_type() == db_type);
        BOOST_CHECK_EQUAL(change.get_database_name(), db_name);
        BOOST_CHECK(change.get_change_type() == change_type);
        BOOST_CHECK(change.get_key() != nullptr);
        BOOST_CHECK_EQUAL(change.get_key()->get_size(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_database_entity_change_modify)
    {
        const celeritas::database_entity_change change{ celeritas::database_type::mysql, "test", celeritas::database_change_type::update_type };

        const celeritas::basis_database db{ "field", "value" };

        auto mutable_change = change;
        mutable_change.modify(db);

        BOOST_CHECK_EQUAL(mutable_change.get_value<celeritas::database_data_type::string_type>("field"), "value");
    }

    BOOST_AUTO_TEST_CASE(test_database_entity_change_get_value)
    {
        celeritas::database_entity_change change{ celeritas::database_type::mysql, "test", celeritas::database_change_type::update_type };
        change.modify(celeritas::basis_database{ "int_field", 100 });

        BOOST_CHECK_EQUAL(change.get_value<celeritas::database_data_type::int32_type>("int_field"), 100);
        BOOST_CHECK_EQUAL(change.get_value<celeritas::database_data_type::int32_type>("missing_field", 5), 5);
    }

    BOOST_AUTO_TEST_CASE(test_database_entity_change_is_modify)
    {
        celeritas::database_entity_change change{ celeritas::database_type::mysql, "test", celeritas::database_change_type::update_type };
        BOOST_CHECK(!change.is_modify());

        change.modify(celeritas::basis_database{ "field", "val" });
        BOOST_CHECK(change.is_modify());
    }

    BOOST_AUTO_TEST_CASE(test_database_entity_change_is_must_save)
    {
        celeritas::database_entity_change change_select{ celeritas::database_type::mysql, "test", celeritas::database_change_type::select_type };
        BOOST_CHECK(!change_select.is_must_save());

        celeritas::database_entity_change change_insert{ celeritas::database_type::mysql, "test", celeritas::database_change_type::insert_type };
        BOOST_CHECK(!change_insert.is_must_save());

        celeritas::database_entity_change change_update{ celeritas::database_type::mysql, "test", celeritas::database_change_type::update_type };
        BOOST_CHECK(!change_update.is_must_save());
        change_update.modify(celeritas::basis_database{ "f", 1 });
        BOOST_CHECK(change_update.is_must_save());
    }

BOOST_AUTO_TEST_SUITE_END()
