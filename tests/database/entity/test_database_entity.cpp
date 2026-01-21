#include "config/basic/database_type.h"
#include "database/basic/basis_database.tpp"
#include "database/basic/database_change_type.h"
#include "database/basic/database_entity_change.h"
#include "database/entity/database_entity.h"
#include "detail/test_entity.h"
#include "fixture/database_entity_fixture.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(database_entity_suite)

    BOOST_FIXTURE_TEST_CASE(test_database_entity_constructor, celeritas::database_entity_fixture)
    {
        const celeritas::database_entity entity{ get_db_type(), get_db_name(), get_key() };

        BOOST_CHECK(!entity.is_modify());
        BOOST_CHECK(!entity.is_must_save());
        BOOST_CHECK(entity.get_modify()->get_change_type() == celeritas::database_change_type::insert_type);
    }

    BOOST_FIXTURE_TEST_CASE(test_database_entity_get_modify, celeritas::database_entity_fixture)
    {
        const celeritas::database_entity entity{ get_db_type(), get_db_name(), get_key() };

        const auto modify = entity.get_modify();
        BOOST_CHECK(modify->get_change_type() == celeritas::database_change_type::insert_type);
        BOOST_CHECK(modify->get_key() == get_key());
        BOOST_CHECK(modify->get_database_type() == get_db_type());
        BOOST_CHECK_EQUAL(modify->get_database_name(), get_db_name());
    }

    BOOST_FIXTURE_TEST_CASE(test_database_entity_get_delete, celeritas::database_entity_fixture)
    {
        const celeritas::database_entity entity{ get_db_type(), get_db_name(), get_key() };

        const auto database_entity_change = entity.get_delete();
        BOOST_CHECK(database_entity_change->get_change_type() == celeritas::database_change_type::delete_type);
        BOOST_CHECK(database_entity_change->get_key() == get_key());
        BOOST_CHECK(database_entity_change->get_database_type() == get_db_type());
        BOOST_CHECK_EQUAL(database_entity_change->get_database_name(), get_db_name());
    }

    BOOST_FIXTURE_TEST_CASE(test_database_entity_clear_modify, celeritas::database_entity_fixture)
    {
        celeritas::database_entity entity{ get_db_type(), get_db_name(), get_key() };

        entity.clear_modify();
        BOOST_CHECK(!entity.is_modify());
    }

    BOOST_FIXTURE_TEST_CASE(test_database_entity_clear_modify_changes_type, celeritas::database_entity_fixture)
    {
        celeritas::database_entity entity{ get_db_type(), get_db_name(), get_key() };
        BOOST_CHECK(entity.get_modify()->get_change_type() == celeritas::database_change_type::insert_type);

        entity.clear_modify();
        BOOST_CHECK(entity.get_modify()->get_change_type() == celeritas::database_change_type::update_type);
    }

    BOOST_FIXTURE_TEST_CASE(test_database_entity_add_modify_on_new_entity, celeritas::database_entity_fixture)
    {
        celeritas::test_entity entity{ get_db_type(), get_db_name(), get_key() };
        BOOST_CHECK(entity.get_modify()->get_change_type() == celeritas::database_change_type::insert_type);

        entity.add_modify("test_field", 123);

        const auto modify = entity.get_modify();
        BOOST_CHECK(modify->get_change_type() == celeritas::database_change_type::insert_type);
        BOOST_CHECK(entity.is_modify());
        BOOST_CHECK(modify->get_database()->get_size() == 1);
    }

    BOOST_FIXTURE_TEST_CASE(test_database_entity_add_modify_after_clear, celeritas::database_entity_fixture)
    {
        celeritas::test_entity entity{ get_db_type(), get_db_name(), get_key() };

        entity.clear_modify();
        BOOST_CHECK(!entity.is_modify());
        BOOST_CHECK(entity.get_modify()->get_change_type() == celeritas::database_change_type::update_type);

        entity.add_modify("test_field", 123);
        BOOST_CHECK(entity.is_modify());
        BOOST_CHECK(entity.is_must_save());

        const auto modify = entity.get_modify();
        BOOST_CHECK(modify->get_change_type() == celeritas::database_change_type::update_type);
        BOOST_CHECK(modify->get_key() == get_key());
        BOOST_CHECK(modify->get_database()->get_size() == 1);
    }

    BOOST_FIXTURE_TEST_CASE(test_database_entity_add_modify_types, celeritas::database_entity_fixture)
    {
        celeritas::test_entity entity{ get_db_type(), get_db_name(), get_key() };
        entity.clear_modify();

        entity.add_modify("int_field", 123);
        entity.add_modify("string_field", std::string("value"));
        entity.add_modify("double_field", 123.456);

        BOOST_CHECK(entity.is_modify());
        const auto modify = entity.get_modify();
        BOOST_CHECK(modify->get_database()->get_size() == 3);
    }

    BOOST_FIXTURE_TEST_CASE(test_database_entity_constructor_from_change, celeritas::database_entity_fixture)
    {
        const celeritas::database_entity_change change{ get_db_type(), get_db_name(), celeritas::database_change_type::update_type, get_key() };
        const celeritas::database_entity entity{ change };

        const auto modify = entity.get_modify();
        BOOST_CHECK(modify->get_database_type() == get_db_type());
        BOOST_CHECK_EQUAL(modify->get_database_name(), get_db_name());
        BOOST_CHECK(modify->get_change_type() == celeritas::database_change_type::update_type);
        BOOST_CHECK(modify->get_key() == get_key());
    }

    BOOST_FIXTURE_TEST_CASE(test_database_entity_constructor_from_type_and_change_same_type, celeritas::database_entity_fixture)
    {
        const celeritas::database_entity_change change{ get_db_type(), get_db_name(), celeritas::database_change_type::update_type, get_key() };

        const celeritas::database_entity entity{ get_db_type(), change };
        const auto modify = entity.get_modify();
        BOOST_CHECK(modify->get_change_type() == celeritas::database_change_type::update_type);
    }

    BOOST_FIXTURE_TEST_CASE(test_database_entity_constructor_from_type_and_change_diff_type, celeritas::database_entity_fixture)
    {
        const celeritas::database_entity_change change{ get_db_type(), get_db_name(), celeritas::database_change_type::update_type, get_key() };

        const auto other_type = get_db_type() == celeritas::database_type::mysql ? celeritas::database_type::redis : celeritas::database_type::mysql;
        const celeritas::database_entity entity{ other_type, change };
        const auto modify = entity.get_modify();

        BOOST_CHECK(modify->get_change_type() == celeritas::database_change_type::insert_type);
        BOOST_CHECK(modify->get_database_type() == other_type);
    }

    BOOST_FIXTURE_TEST_CASE(test_database_entity_copy_constructor, celeritas::database_entity_fixture)
    {
        celeritas::test_entity entity{ get_db_type(), get_db_name(), get_key() };
        entity.add_modify("field", 1);

        auto entity2{ entity };
        BOOST_CHECK(entity2.is_modify());
        BOOST_CHECK(entity2.get_modify() == entity.get_modify());

        entity2.add_modify("field2", 2);
        BOOST_CHECK(entity2.get_modify() != entity.get_modify());
        BOOST_CHECK(entity.get_modify()->get_database()->get_size() == 1);
        BOOST_CHECK(entity2.get_modify()->get_database()->get_size() == 2);
    }

    BOOST_FIXTURE_TEST_CASE(test_database_entity_copy_assignment, celeritas::database_entity_fixture)
    {
        celeritas::test_entity entity1{ get_db_type(), get_db_name(), get_key() };
        entity1.add_modify("field", 1);

        celeritas::test_entity entity2{ get_db_type(), "other_db", get_key() };
        entity2 = entity1;
        BOOST_CHECK(entity2.is_modify());
        BOOST_CHECK(entity2.get_modify() == entity1.get_modify());

        entity2.add_modify("field2", 2);
        BOOST_CHECK(entity2.get_modify() != entity1.get_modify());
        BOOST_CHECK(entity1.get_modify()->get_database()->get_size() == 1);
        BOOST_CHECK(entity2.get_modify()->get_database()->get_size() == 2);
    }

    BOOST_FIXTURE_TEST_CASE(test_database_entity_move_constructor, celeritas::database_entity_fixture)
    {
        celeritas::test_entity entity1{ get_db_type(), get_db_name(), get_key() };
        entity1.add_modify("field", 1);
        const auto modify_ptr = entity1.get_modify();

        const auto entity2{ std::move(entity1) };
        BOOST_CHECK(entity2.is_modify());
        BOOST_CHECK(entity2.get_modify() == modify_ptr);
    }

    BOOST_FIXTURE_TEST_CASE(test_database_entity_move_assignment, celeritas::database_entity_fixture)
    {
        celeritas::test_entity entity1{ get_db_type(), get_db_name(), get_key() };
        entity1.add_modify("field", 1);
        const auto modify_ptr = entity1.get_modify();

        celeritas::test_entity entity2{ get_db_type(), "other_db", get_key() };
        entity2 = std::move(entity1);
        BOOST_CHECK(entity2.is_modify());
        BOOST_CHECK(entity2.get_modify() == modify_ptr);
    }

    BOOST_FIXTURE_TEST_CASE(test_database_entity_add_modify_basis_database, celeritas::database_entity_fixture)
    {
        celeritas::test_entity entity{ get_db_type(), get_db_name(), get_key() };
        entity.clear_modify();

        const celeritas::basis_database basis_database{ "test_field", 123 };
        entity.add_modify(basis_database);

        BOOST_CHECK(entity.is_modify());
        const auto modify = entity.get_modify();
        BOOST_CHECK(modify->get_database()->get_size() == 1);
    }

    BOOST_FIXTURE_TEST_CASE(test_database_entity_is_must_save, celeritas::database_entity_fixture)
    {
        celeritas::test_entity entity{ get_db_type(), get_db_name(), get_key() };
        BOOST_CHECK(!entity.is_must_save());

        entity.add_modify("field1", 1);
        entity.add_modify("field2", 2);
        BOOST_CHECK(entity.is_must_save());

        entity.clear_modify();
        BOOST_CHECK(!entity.is_must_save());
    }

BOOST_AUTO_TEST_SUITE_END()
