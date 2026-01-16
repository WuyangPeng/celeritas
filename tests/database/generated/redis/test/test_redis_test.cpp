#include "config/basic/database_type.h"
#include "database/basic/basis_database.h"
#include "database/basic/basis_database_container.h"
#include "database/basic/database_change_type.h"
#include "database/basic/database_entity_change.h"
#include "database/detail/check_helper.h"
#include "database/document/test/logs_data.h"
#include "database/document/test/properties_data.h"
#include "database/entity/database_entity.h"
#include "database/generated/redis/test/redis_test.h"

#include <boost/test/unit_test.hpp>

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

namespace
{
    [[nodiscard]] celeritas::properties_data get_test_properties_data()
    {
        celeritas::properties_data data{};
        data.set_int64_value(111111LL);
        data.set_string_value("complete_test_string");
        data.set_string_array_value({ "str1", "str2", "str3" });
        data.set_int32_value(222);
        data.set_int32_count_value(333);
        data.set_int32_array_value({ 444, 555, 666 });
        data.set_int64_count_value(777777LL);
        data.set_int64_array_value({ 888888LL, 999999LL });
        data.set_double_value(1.2345);
        data.set_double_array_value({ 6.7, 8.9, 10.11 });
        data.set_bool_value(true);

        const celeritas::properties_data::document_type nested_document{ celeritas::basis_database{ "nested_int", 100 },
                                                                         celeritas::basis_database{ "nested_string", "hello" } };

        data.set_document_value(nested_document);

        celeritas::properties_data::document_type document_array_element1{ celeritas::basis_database{ "array_int1", 1 } };
        celeritas::properties_data::document_type document_array_element2{ celeritas::basis_database{ "array_string2", "world" } };

        data.set_document_array_value({ document_array_element1, document_array_element2 });

        return data;
    }

    [[nodiscard]] celeritas::logs_data get_test_logs_data()
    {
        celeritas::logs_data data{};
        data.set_int64_value(111111LL);
        data.set_string_value("complete_test_string");
        data.set_string_array_value({ "str1", "str2", "str3" });
        data.set_int32_value(222);
        data.set_int32_count_value(333);
        data.set_int32_array_value({ 444, 555, 666 });
        data.set_int64_count_value(777777LL);
        data.set_int64_array_value({ 888888LL, 999999LL });
        data.set_double_value(1.2345);
        data.set_double_array_value({ 6.7, 8.9, 10.11 });
        data.set_bool_value(true);

        const celeritas::logs_data::document_type nested_document{ celeritas::basis_database{ "nested_int", 100 },
                                                                   celeritas::basis_database{ "nested_string", "hello" } };

        data.set_document_value(nested_document);

        celeritas::logs_data::document_type document_array_element1{ celeritas::basis_database{ "array_int1", 1 } };
        celeritas::logs_data::document_type document_array_element2{ celeritas::basis_database{ "array_string2", "world" } };

        data.set_document_array_value({ document_array_element1, document_array_element2 });

        return data;
    }
}

BOOST_AUTO_TEST_SUITE(redis_test_suite)

    BOOST_AUTO_TEST_CASE(test_constructors)
    {
        const auto key_container = std::make_shared<const celeritas::basis_database_container>(celeritas::basis_database{ "id", 1 });
        const celeritas::database_entity_change entity_change{ celeritas::database_type::redis,
                                                               "redis_test",
                                                               celeritas::database_change_type::insert_type,
                                                               key_container };

        const celeritas::redis_test redis_test1{ entity_change };
        const celeritas::redis_test redis_test2{ celeritas::database_type::redis, entity_change };
        const celeritas::redis_test redis_test3{ celeritas::database_type::redis, 123456LL };

        BOOST_CHECK_EQUAL(redis_test3.get_user_id(), 123456LL);
    }

    BOOST_AUTO_TEST_CASE(test_getters_and_setters)
    {
        const auto key_container = std::make_shared<const celeritas::basis_database_container>(celeritas::basis_database{ "id", 1 });
        const celeritas::database_entity_change entity_change{ celeritas::database_type::redis,
                                                               "redis_test",
                                                               celeritas::database_change_type::insert_type,
                                                               key_container };

        celeritas::redis_test redis_test{ entity_change };

        redis_test.set_user_id(111111LL);
        BOOST_CHECK_EQUAL(redis_test.get_user_id(), 111111LL);

        redis_test.set_chapter_id(222);
        BOOST_CHECK_EQUAL(redis_test.get_chapter_id(), 222);

        redis_test.set_chapter_name("test_chapter");
        BOOST_CHECK_EQUAL(redis_test.get_chapter_name(), "test_chapter");

        redis_test.set_chance_winning(0.75);
        BOOST_CHECK_CLOSE(redis_test.get_chance_winning(), 0.75, 0.001);

        redis_test.set_winning(true);
        BOOST_CHECK_EQUAL(redis_test.is_winning(), true);

        redis_test.set_currency(5000LL);
        BOOST_CHECK_EQUAL(redis_test.get_currency(), 5000LL);

        redis_test.set_count(100);
        BOOST_CHECK_EQUAL(redis_test.get_count(), 100);
    }

    BOOST_AUTO_TEST_CASE(test_array_field_operations)
    {
        const auto key_container = std::make_shared<const celeritas::basis_database_container>(celeritas::basis_database{ "id", 1 });
        const celeritas::database_entity_change entity_change{ celeritas::database_type::redis,
                                                               "redis_test",
                                                               celeritas::database_change_type::insert_type,
                                                               key_container };

        celeritas::redis_test redis_test{ entity_change };

        const celeritas::traits::string_array_type tags{ "tag1", "tag2", "tag3" };
        redis_test.set_tags(tags);
        celeritas::check_array(tags, redis_test.get_tags());

        const celeritas::traits::int32_array_type category_indices{ 1, 2, 3, 4 };
        redis_test.set_category_index(category_indices);
        celeritas::check_array(category_indices, redis_test.get_category_index());

        const celeritas::traits::int64_array_type related_indices{ 100LL, 200LL, 300LL };
        redis_test.set_related_index(related_indices);
        celeritas::check_array(related_indices, redis_test.get_related_index());

        const celeritas::traits::double_array_type ratios{ 0.1, 0.2, 0.3 };
        redis_test.set_ratios(ratios);
        celeritas::check_array(ratios, redis_test.get_ratios());
    }

    BOOST_AUTO_TEST_CASE(test_byte_array_operations)
    {
        const auto key_container = std::make_shared<const celeritas::basis_database_container>(celeritas::basis_database{ "id", 1 });
        const celeritas::database_entity_change entity_change{ celeritas::database_type::redis,
                                                               "redis_test",
                                                               celeritas::database_change_type::insert_type,
                                                               key_container };

        celeritas::redis_test redis_test{ entity_change };

        const celeritas::traits::byte_array_type bytes{ 'h', 'e', 'l', 'l', 'o' };
        redis_test.set_attachment(bytes);
        celeritas::check_array(bytes, redis_test.get_attachment());
    }

    BOOST_AUTO_TEST_CASE(test_document_field_operations)
    {
        const auto key_container = std::make_shared<const celeritas::basis_database_container>(celeritas::basis_database{ "id", 1 });
        const celeritas::database_entity_change entity_change{ celeritas::database_type::redis,
                                                               "redis_test",
                                                               celeritas::database_change_type::insert_type,
                                                               key_container };

        celeritas::redis_test redis_test{ entity_change };

        const auto properties_data = get_test_properties_data();
        redis_test.set_properties(properties_data.to_document_type());
        check_properties_data(celeritas::properties_data::from_document(redis_test.get_properties()), properties_data);

        const auto logs_data = get_test_logs_data();
        redis_test.add_logs(logs_data.to_document_type());
        BOOST_CHECK_EQUAL(redis_test.get_logs().size(), 1);
        check_logs_data(celeritas::logs_data::from_document(redis_test.get_logs().at(0)), logs_data);
    }

    BOOST_AUTO_TEST_CASE(test_modifier_methods)
    {
        const auto key_container = std::make_shared<const celeritas::basis_database_container>(celeritas::basis_database{ "id", 1 });
        const celeritas::database_entity_change entity_change{ celeritas::database_type::redis,
                                                               "redis_test",
                                                               celeritas::database_change_type::insert_type,
                                                               key_container };

        celeritas::redis_test redis_test{ entity_change };

        redis_test.set_currency(1000LL);
        redis_test.modify_currency(500LL);
        BOOST_CHECK_EQUAL(redis_test.get_currency(), 1500LL);

        redis_test.set_count(50);
        redis_test.modify_count(10);
        BOOST_CHECK_EQUAL(redis_test.get_count(), 60);
    }

    BOOST_AUTO_TEST_CASE(test_tags_element_operations)
    {
        const auto key_container = std::make_shared<const celeritas::basis_database_container>(celeritas::basis_database{ "id", 1 });
        const celeritas::database_entity_change entity_change{ celeritas::database_type::redis,
                                                               "redis_test",
                                                               celeritas::database_change_type::insert_type,
                                                               key_container };

        celeritas::redis_test redis_test{ entity_change };
        redis_test.add_tags("new_tag");
        BOOST_CHECK_EQUAL(redis_test.get_tags().size(), 1);
        BOOST_CHECK_EQUAL(redis_test.get_tags().at(0), "new_tag");
        redis_test.set_tags(0, "updated_tag");
        BOOST_CHECK_EQUAL(redis_test.get_tags().at(0), "updated_tag");
    }

    BOOST_AUTO_TEST_CASE(test_category_index_element_operations)
    {
        const auto key_container = std::make_shared<const celeritas::basis_database_container>(celeritas::basis_database{ "id", 1 });
        const celeritas::database_entity_change entity_change{ celeritas::database_type::redis,
                                                               "redis_test",
                                                               celeritas::database_change_type::insert_type,
                                                               key_container };

        celeritas::redis_test redis_test{ entity_change };
        redis_test.add_category_index(999);
        BOOST_CHECK_EQUAL(redis_test.get_category_index().size(), 1);
        BOOST_CHECK_EQUAL(redis_test.get_category_index().at(0), 999);
        redis_test.set_category_index(0, 888);
        BOOST_CHECK_EQUAL(redis_test.get_category_index().at(0), 888);
    }

    BOOST_AUTO_TEST_CASE(test_related_index_element_operations)
    {
        const auto key_container = std::make_shared<const celeritas::basis_database_container>(celeritas::basis_database{ "id", 1 });
        const celeritas::database_entity_change entity_change{ celeritas::database_type::redis,
                                                               "redis_test",
                                                               celeritas::database_change_type::insert_type,
                                                               key_container };

        celeritas::redis_test redis_test{ entity_change };
        redis_test.add_related_index(777777LL);
        BOOST_CHECK_EQUAL(redis_test.get_related_index().size(), 1);
        BOOST_CHECK_EQUAL(redis_test.get_related_index().at(0), 777777LL);
        redis_test.set_related_index(0, 666666LL);
        BOOST_CHECK_EQUAL(redis_test.get_related_index().at(0), 666666LL);
    }

    BOOST_AUTO_TEST_CASE(test_ratios_element_operations)
    {
        const auto key_container = std::make_shared<const celeritas::basis_database_container>(celeritas::basis_database{ "id", 1 });
        const celeritas::database_entity_change entity_change{ celeritas::database_type::redis,
                                                               "redis_test",
                                                               celeritas::database_change_type::insert_type,
                                                               key_container };

        celeritas::redis_test redis_test{ entity_change };
        redis_test.add_ratios(0.99);
        BOOST_CHECK_EQUAL(redis_test.get_ratios().size(), 1);
        BOOST_CHECK_CLOSE(redis_test.get_ratios().at(0), 0.99, 0.001);
        redis_test.set_ratios(0, 0.88);
        BOOST_CHECK_CLOSE(redis_test.get_ratios().at(0), 0.88, 0.001);
    }

    BOOST_AUTO_TEST_CASE(test_attachment_element_operations)
    {
        const auto key_container = std::make_shared<const celeritas::basis_database_container>(celeritas::basis_database{ "id", 1 });
        const celeritas::database_entity_change entity_change{ celeritas::database_type::redis,
                                                               "redis_test",
                                                               celeritas::database_change_type::insert_type,
                                                               key_container };

        celeritas::redis_test redis_test{ entity_change };
        redis_test.add_attachment('x');
        BOOST_CHECK_EQUAL(redis_test.get_attachment().size(), 1);
        BOOST_CHECK_EQUAL(redis_test.get_attachment().at(0), 'x');
        redis_test.set_attachment(0, 'y');
        BOOST_CHECK_EQUAL(redis_test.get_attachment().at(0), 'y');
    }

    BOOST_AUTO_TEST_CASE(test_logs_element_operations)
    {
        const auto key_container = std::make_shared<const celeritas::basis_database_container>(celeritas::basis_database{ "id", 1 });
        const celeritas::database_entity_change entity_change{ celeritas::database_type::redis,
                                                               "redis_test",
                                                               celeritas::database_change_type::insert_type,
                                                               key_container };

        celeritas::redis_test redis_test{ entity_change };
        celeritas::logs_data logs_data1{};
        logs_data1.set_string_value("log1");
        redis_test.add_logs(logs_data1.to_document_type());
        BOOST_CHECK_EQUAL(redis_test.get_logs().size(), 1);
        const auto retrieved_log1 = celeritas::logs_data::from_document(redis_test.get_logs().at(0));
        BOOST_CHECK_EQUAL(retrieved_log1.get_string_value(), "log1");

        celeritas::logs_data logs_data2{};
        logs_data2.set_string_value("log2");
        redis_test.set_logs(0, logs_data2.to_document_type());
        BOOST_CHECK_EQUAL(redis_test.get_logs().size(), 1);
        const auto retrieved_log2 = celeritas::logs_data::from_document(redis_test.get_logs().at(0));
        BOOST_CHECK_EQUAL(retrieved_log2.get_string_value(), "log2");
    }

    BOOST_AUTO_TEST_CASE(test_static_methods)
    {
        const auto key_container = std::make_shared<const celeritas::basis_database_container>(celeritas::basis_database{ "id", 1 });
        const celeritas::database_entity_change entity_change{ celeritas::database_type::redis,
                                                               "redis_test",
                                                               celeritas::database_change_type::insert_type,
                                                               key_container };

        const auto& field_container = celeritas::redis_test::get_database_field_container();
        BOOST_CHECK(!field_container.empty());

        const auto select1 = celeritas::redis_test::get_select(celeritas::database_type::redis);
        BOOST_CHECK_EQUAL(select1->get_database_name(), "redis_test");

        const auto select2 = celeritas::redis_test::get_select(celeritas::database_type::redis, 123456LL);
        BOOST_CHECK_EQUAL(select2->get_database_name(), "redis_test");
    }

    BOOST_AUTO_TEST_CASE(test_constants)
    {
        BOOST_CHECK_EQUAL(celeritas::redis_test::database_name, "redis_test");
        BOOST_CHECK_EQUAL(celeritas::redis_test::user_id_describe, "user_id");
        BOOST_CHECK_EQUAL(celeritas::redis_test::chapter_id_describe, "chapter_id");
        BOOST_CHECK_EQUAL(celeritas::redis_test::chapter_name_describe, "chapter_name");
        BOOST_CHECK_EQUAL(celeritas::redis_test::chance_winning_describe, "chance_winning");
        BOOST_CHECK_EQUAL(celeritas::redis_test::winning_describe, "winning");
        BOOST_CHECK_EQUAL(celeritas::redis_test::currency_describe, "currency");
        BOOST_CHECK_EQUAL(celeritas::redis_test::count_describe, "count");
        BOOST_CHECK_EQUAL(celeritas::redis_test::tags_describe, "tags");
        BOOST_CHECK_EQUAL(celeritas::redis_test::category_index_describe, "category_index");
        BOOST_CHECK_EQUAL(celeritas::redis_test::related_index_describe, "related_index");
        BOOST_CHECK_EQUAL(celeritas::redis_test::ratios_describe, "ratios");
        BOOST_CHECK_EQUAL(celeritas::redis_test::attachment_describe, "attachment");
        BOOST_CHECK_EQUAL(celeritas::redis_test::properties_describe, "properties");
        BOOST_CHECK_EQUAL(celeritas::redis_test::logs_describe, "logs");
    }

    BOOST_AUTO_TEST_CASE(test_inheritance_modify)
    {
        const auto key_container = std::make_shared<const celeritas::basis_database_container>(celeritas::basis_database{ "id", 1 });
        const celeritas::database_entity_change entity_change{ celeritas::database_type::redis,
                                                               "redis_test",
                                                               celeritas::database_change_type::insert_type,
                                                               key_container };

        celeritas::redis_test redis_test{ entity_change };

        BOOST_CHECK(!redis_test.is_modify());
        BOOST_CHECK(!redis_test.is_must_save());
        redis_test.set_chapter_name("new name");

        BOOST_CHECK(redis_test.is_modify());
        BOOST_CHECK(redis_test.is_must_save());

        const auto modify = redis_test.get_modify();
        BOOST_CHECK(modify);
        BOOST_CHECK_EQUAL(modify->get_database_name(), "redis_test");
        BOOST_CHECK(modify->get_change_type() == celeritas::database_change_type::update_type);

        redis_test.clear_modify();
        BOOST_CHECK(!redis_test.is_modify());
        BOOST_CHECK(!redis_test.is_must_save());
    }

    BOOST_AUTO_TEST_CASE(test_inheritance_delete)
    {
        const auto key_container = std::make_shared<const celeritas::basis_database_container>(celeritas::basis_database{ "id", 1 });
        const celeritas::database_entity_change entity_change{ celeritas::database_type::redis,
                                                               "redis_test",
                                                               celeritas::database_change_type::insert_type,
                                                               key_container };

        const celeritas::redis_test redis_test{ entity_change };

        const auto delete_redis = redis_test.get_delete();
        BOOST_CHECK(delete_redis);
        BOOST_CHECK_EQUAL(delete_redis->get_database_name(), "redis_test");
        BOOST_CHECK(delete_redis->get_change_type() == celeritas::database_change_type::delete_type);
    }

BOOST_AUTO_TEST_SUITE_END()
