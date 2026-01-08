#include "common/core/celeritas_error.h"
#include "config/basic/database_type.h"
#include "database/database_constant.h"
#include "database/basic/database_change_type.h"
#include "database/basic/database_entity_change.h"

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
        BOOST_CHECK_EQUAL(change.get_database()->get_size(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_database_entity_change_constructor_with_composite_key)
    {
        constexpr auto db_type = celeritas::database_type::mysql;
        const std::string db_name{ "test_db" };
        constexpr auto change_type = celeritas::database_change_type::insert_type;
        const auto key = std::make_shared<celeritas::basis_database_container>();
        key->modify(celeritas::basis_database{ "id", 1 });
        key->modify(celeritas::basis_database{ "name", "test" });

        const celeritas::database_entity_change change{ db_type, db_name, change_type, key };

        BOOST_CHECK(change.get_database_type() == db_type);
        BOOST_CHECK_EQUAL(change.get_database_name(), db_name);
        BOOST_CHECK(change.get_change_type() == change_type);
        BOOST_CHECK(change.get_key() == key);
        BOOST_CHECK(change.get_database() != nullptr);
        BOOST_CHECK_EQUAL(change.get_database()->get_size(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_database_entity_change_constructor_without_key)
    {
        constexpr auto db_type = celeritas::database_type::redis;
        const std::string db_name{ "test_redis" };

        BOOST_CHECK_THROW((celeritas::database_entity_change{ db_type, db_name, celeritas::database_change_type::update_type }), celeritas::celeritas_error);
        BOOST_CHECK_THROW((celeritas::database_entity_change{ db_type, db_name, celeritas::database_change_type::insert_type }), celeritas::celeritas_error);
        BOOST_CHECK_THROW((celeritas::database_entity_change{ db_type, db_name, celeritas::database_change_type::delete_type }), celeritas::celeritas_error);
    }

    BOOST_AUTO_TEST_CASE(test_database_entity_change_constructor_without_key_select_type)
    {
        constexpr auto db_type = celeritas::database_type::redis;
        const std::string db_name{ "test_redis" };
        constexpr auto change_type = celeritas::database_change_type::select_type;

        const celeritas::database_entity_change change{ db_type, db_name, change_type };

        BOOST_CHECK(change.get_database_type() == db_type);
        BOOST_CHECK_EQUAL(change.get_database_name(), db_name);
        BOOST_CHECK(change.get_change_type() == change_type);
        BOOST_CHECK(change.get_key() != nullptr);
        BOOST_CHECK_EQUAL(change.get_key()->get_size(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_database_entity_change_get_select)
    {
        const celeritas::database_entity_change original_change{ celeritas::database_type::mysql, "test_db", celeritas::database_change_type::select_type };
        const auto select_change = original_change.get_select();

        BOOST_CHECK(select_change.get_database_type() == original_change.get_database_type());
        BOOST_CHECK_EQUAL(select_change.get_database_name(), original_change.get_database_name());
        BOOST_CHECK(select_change.get_change_type() == celeritas::database_change_type::select_type);
        BOOST_CHECK(select_change.get_key() == original_change.get_key());
    }

    BOOST_AUTO_TEST_CASE(test_database_entity_change_get_select_with_key)
    {
        const celeritas::database_entity_change original_change{ celeritas::database_type::mysql, "test_db", celeritas::database_change_type::select_type };
        const auto new_key = std::make_shared<celeritas::basis_database_container>(celeritas::basis_database{ "id", 2 });
        const auto select_change = original_change.get_select(new_key);

        BOOST_CHECK(select_change.get_database_type() == original_change.get_database_type());
        BOOST_CHECK_EQUAL(select_change.get_database_name(), original_change.get_database_name());
        BOOST_CHECK(select_change.get_change_type() == celeritas::database_change_type::select_type);
        BOOST_CHECK(select_change.get_key() == new_key);
    }

    BOOST_AUTO_TEST_CASE(test_database_entity_change_modify)
    {
        const auto key = std::make_shared<celeritas::basis_database_container>(celeritas::basis_database{ "id", 1 });
        const celeritas::database_entity_change change{ celeritas::database_type::mysql, "test", celeritas::database_change_type::update_type, key };

        const celeritas::basis_database db{ "field", "value" };

        auto mutable_change = change;
        mutable_change.modify(db);

        BOOST_CHECK_EQUAL(mutable_change.get_value<celeritas::database_data_type::string_type>("field"), "value");
    }

    BOOST_AUTO_TEST_CASE(test_database_entity_change_modify_overwrite)
    {
        const auto key = std::make_shared<celeritas::basis_database_container>(celeritas::basis_database{ "id", 1 });
        celeritas::database_entity_change change{ celeritas::database_type::mysql, "test", celeritas::database_change_type::update_type, key };
        change.modify(celeritas::basis_database{ "field", "original_value" });
        change.modify(celeritas::basis_database{ "field", "new_value" });

        BOOST_CHECK_EQUAL(change.get_value<celeritas::database_data_type::string_type>("field"), "new_value");
    }

    BOOST_AUTO_TEST_CASE(test_database_entity_change_clear)
    {
        const auto key = std::make_shared<celeritas::basis_database_container>(celeritas::basis_database{ "id", 1 });
        celeritas::database_entity_change change{ celeritas::database_type::mysql, "test", celeritas::database_change_type::update_type, key };
        change.modify(celeritas::basis_database{ "field", "value" });
        BOOST_CHECK(change.is_modify());

        change.clear();
        BOOST_CHECK(!change.is_modify());
    }

    BOOST_AUTO_TEST_CASE(test_database_entity_change_clear_side_effect)
    {
        const auto key = std::make_shared<celeritas::basis_database_container>(celeritas::basis_database{ "id", 1 });
        celeritas::database_entity_change change{ celeritas::database_type::mysql, "test", celeritas::database_change_type::insert_type, key };
        BOOST_CHECK(change.get_change_type() == celeritas::database_change_type::insert_type);

        change.clear();
        BOOST_CHECK(change.get_change_type() == celeritas::database_change_type::update_type);
    }

    BOOST_AUTO_TEST_CASE(test_database_entity_change_clear_on_insert_state_transition)
    {
        const auto key = std::make_shared<celeritas::basis_database_container>(celeritas::basis_database{ "id", 1 });
        celeritas::database_entity_change change{ celeritas::database_type::mysql, "test", celeritas::database_change_type::insert_type, key };

        change.modify(celeritas::basis_database{ "id", 1 });
        BOOST_CHECK(change.get_change_type() == celeritas::database_change_type::insert_type);
        BOOST_CHECK(change.is_modify());
        BOOST_CHECK(!change.is_must_save());

        change.clear();

        BOOST_CHECK(change.get_change_type() == celeritas::database_change_type::update_type);
        BOOST_CHECK(!change.is_modify());
        BOOST_CHECK(!change.is_must_save());
    }

    BOOST_AUTO_TEST_CASE(test_database_entity_change_get_value)
    {
        const auto key = std::make_shared<celeritas::basis_database_container>(celeritas::basis_database{ "id", 1 });
        celeritas::database_entity_change change{ celeritas::database_type::mysql, "test", celeritas::database_change_type::update_type, key };
        change.modify(celeritas::basis_database{ "int_field", 100 });

        BOOST_CHECK_EQUAL(change.get_value<celeritas::database_data_type::int32_type>("int_field"), 100);
        BOOST_CHECK_EQUAL(change.get_value<celeritas::database_data_type::int32_type>("missing_field", 5), 5);
    }

    BOOST_AUTO_TEST_CASE(test_database_entity_change_get_value_type_mismatch)
    {
        const auto key = std::make_shared<celeritas::basis_database_container>(celeritas::basis_database{ "id", 1 });
        celeritas::database_entity_change change{ celeritas::database_type::mysql, "test", celeritas::database_change_type::update_type, key };
        change.modify(celeritas::basis_database{ "int_field", 100 });

        BOOST_CHECK_THROW([&change]{ std::ignore = change.get_value<celeritas::database_data_type::string_type>("int_field"); }(), std::bad_variant_access);
    }

    BOOST_AUTO_TEST_CASE(test_database_entity_change_get_database_content)
    {
        const auto key = std::make_shared<celeritas::basis_database_container>(celeritas::basis_database{ "id", 1 });
        celeritas::database_entity_change change{ celeritas::database_type::mysql, "test", celeritas::database_change_type::update_type, key };
        change.modify(celeritas::basis_database{ "field1", "value1" });
        change.modify(celeritas::basis_database{ "field2", 123 });

        const auto db = change.get_database();
        BOOST_CHECK(db != nullptr);
        BOOST_CHECK_EQUAL(db->get_size(), 2);

        BOOST_CHECK_EQUAL(change.get_value<celeritas::database_data_type::string_type>("field1"), "value1");
        BOOST_CHECK_EQUAL(change.get_value<celeritas::database_data_type::int32_type>("field2"), 123);
    }

    BOOST_AUTO_TEST_CASE(test_database_entity_change_is_modify)
    {
        const auto key = std::make_shared<celeritas::basis_database_container>(celeritas::basis_database{ "id", 1 });
        celeritas::database_entity_change change{ celeritas::database_type::mysql, "test", celeritas::database_change_type::update_type, key };
        BOOST_CHECK(!change.is_modify());

        change.modify(celeritas::basis_database{ "field", "val" });
        BOOST_CHECK(change.is_modify());

        const celeritas::database_entity_change delete_change{ celeritas::database_type::mysql, "test", celeritas::database_change_type::delete_type, key };
        BOOST_CHECK(delete_change.is_modify());
    }

    BOOST_AUTO_TEST_CASE(test_database_entity_change_is_must_save)
    {
        const celeritas::database_entity_change change_select{ celeritas::database_type::mysql, "test", celeritas::database_change_type::select_type };
        BOOST_CHECK(!change_select.is_must_save());

        const auto key = std::make_shared<celeritas::basis_database_container>(celeritas::basis_database{ "id", 1 });
        const celeritas::database_entity_change change_insert{ celeritas::database_type::mysql, "test", celeritas::database_change_type::insert_type, key };
        BOOST_CHECK(!change_insert.is_must_save());

        celeritas::database_entity_change change_update{ celeritas::database_type::mysql, "test", celeritas::database_change_type::update_type, key };
        BOOST_CHECK(!change_update.is_must_save());
        change_update.modify(celeritas::basis_database{ "f", 1 });
        BOOST_CHECK(change_update.is_must_save());

        const celeritas::database_entity_change change_delete{ celeritas::database_type::mysql, "test", celeritas::database_change_type::delete_type, key };
        BOOST_CHECK(change_delete.is_must_save());

        celeritas::database_entity_change change_insert_modified{ celeritas::database_type::mysql, "test", celeritas::database_change_type::insert_type, key };
        change_insert_modified.modify(celeritas::basis_database{ "id", 1 });
        BOOST_CHECK(!change_insert_modified.is_must_save());

        change_insert_modified.modify(celeritas::basis_database{ "f", 1 });
        BOOST_CHECK(change_insert_modified.is_must_save());
    }

BOOST_AUTO_TEST_SUITE_END()
