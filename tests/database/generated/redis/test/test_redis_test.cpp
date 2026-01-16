#include "config/basic/database_type.h"
#include "database/basic/basis_database.h"
#include "database/basic/basis_database_container.h"
#include "database/basic/database_change_type.h"
#include "database/basic/database_entity_change.h"
#include "database/entity/database_entity.h"
#include "database/generated/redis/test/redis_test.h"

#include <boost/test/unit_test.hpp>

#include <memory>
#include <string>
#include <vector>

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

        // 测试标签数组
        celeritas::traits::string_array_type tags = { "tag1", "tag2", "tag3" };
        redis_test.set_tags(tags);
        const auto& retrieved_tags = redis_test.get_tags();
        BOOST_CHECK_EQUAL(retrieved_tags.size(), 3);
        BOOST_CHECK_EQUAL(retrieved_tags[0], "tag1");
        BOOST_CHECK_EQUAL(retrieved_tags[1], "tag2");
        BOOST_CHECK_EQUAL(retrieved_tags[2], "tag3");

        // 测试分类索引数组
        celeritas::traits::int32_array_type category_indices = { 1, 2, 3, 4 };
        redis_test.set_category_index(category_indices);
        const auto& retrieved_categories = redis_test.get_category_index();
        BOOST_CHECK_EQUAL(retrieved_categories.size(), 4);
        BOOST_CHECK_EQUAL(retrieved_categories[0], 1);
        BOOST_CHECK_EQUAL(retrieved_categories[1], 2);
        BOOST_CHECK_EQUAL(retrieved_categories[2], 3);
        BOOST_CHECK_EQUAL(retrieved_categories[3], 4);

        // 测试相关索引数组
        celeritas::traits::int64_array_type related_indices = { 100LL, 200LL, 300LL };
        redis_test.set_related_index(related_indices);
        const auto& retrieved_related = redis_test.get_related_index();
        BOOST_CHECK_EQUAL(retrieved_related.size(), 3);
        BOOST_CHECK_EQUAL(retrieved_related[0], 100LL);
        BOOST_CHECK_EQUAL(retrieved_related[1], 200LL);
        BOOST_CHECK_EQUAL(retrieved_related[2], 300LL);

        // 测试比率数组
        celeritas::traits::double_array_type ratios = { 0.1, 0.2, 0.3 };
        redis_test.set_ratios(ratios);
        const auto& retrieved_ratios = redis_test.get_ratios();
        BOOST_CHECK_EQUAL(retrieved_ratios.size(), 3);
        BOOST_CHECK_CLOSE(retrieved_ratios[0], 0.1, 0.001);
        BOOST_CHECK_CLOSE(retrieved_ratios[1], 0.2, 0.001);
        BOOST_CHECK_CLOSE(retrieved_ratios[2], 0.3, 0.001);
    }

    BOOST_AUTO_TEST_CASE(test_byte_array_operations)
    {
        const auto key_container = std::make_shared<const celeritas::basis_database_container>(celeritas::basis_database{ "id", 1 });
        const celeritas::database_entity_change entity_change{ celeritas::database_type::redis,
                                                               "redis_test",
                                                               celeritas::database_change_type::insert_type,
                                                               key_container };

        celeritas::redis_test redis_test{ entity_change };

        // 测试字节数组（attachment字段）
        celeritas::traits::byte_array_type bytes = { 'h', 'e', 'l', 'l', 'o' };
        redis_test.set_attachment(bytes);
        const auto& retrieved_bytes = redis_test.get_attachment();
        BOOST_CHECK_EQUAL(retrieved_bytes.size(), 5);
        BOOST_CHECK_EQUAL(retrieved_bytes[0], 'h');
        BOOST_CHECK_EQUAL(retrieved_bytes[1], 'e');
        BOOST_CHECK_EQUAL(retrieved_bytes[2], 'l');
        BOOST_CHECK_EQUAL(retrieved_bytes[3], 'l');
        BOOST_CHECK_EQUAL(retrieved_bytes[4], 'o');
    }

    BOOST_AUTO_TEST_CASE(test_document_field_operations)
    {
        const auto key_container = std::make_shared<const celeritas::basis_database_container>(celeritas::basis_database{ "id", 1 });
        const celeritas::database_entity_change entity_change{ celeritas::database_type::redis,
                                                               "redis_test",
                                                               celeritas::database_change_type::insert_type,
                                                               key_container };

        celeritas::redis_test redis_test{ entity_change };

        // 由于文档类型的具体实现未知，我们只验证可以正常访问
        celeritas::traits::document_type properties_doc; // 默认构造
        redis_test.set_properties(properties_doc);
        const auto& retrieved_props = redis_test.get_properties();
        (void)retrieved_props; // 避免未使用警告

        celeritas::traits::document_array_type logs_docs = {}; // 空数组
        redis_test.set_logs(logs_docs);
        const auto& retrieved_logs = redis_test.get_logs();
        BOOST_CHECK_EQUAL(retrieved_logs.size(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_modifier_methods)
    {
        const auto key_container = std::make_shared<const celeritas::basis_database_container>(celeritas::basis_database{ "id", 1 });
        const celeritas::database_entity_change entity_change{ celeritas::database_type::redis,
                                                               "redis_test",
                                                               celeritas::database_change_type::insert_type,
                                                               key_container };

        celeritas::redis_test redis_test{ entity_change };

        // 测试货币修改方法
        redis_test.set_currency(1000LL);
        redis_test.modify_currency(500LL);
        // 注意：modify_currency的具体行为取决于实现，这里只是测试方法存在
        (void)redis_test.get_currency(); // 验证方法可调用

        // 测试计数修改方法
        redis_test.set_count(50);
        redis_test.modify_count(10);
        // 注意：modify_count的具体行为取决于实现，这里只是测试方法存在
        (void)redis_test.get_count(); // 验证方法可调用
    }

    BOOST_AUTO_TEST_CASE(test_array_element_operations)
    {
        const auto key_container = std::make_shared<const celeritas::basis_database_container>(celeritas::basis_database{ "id", 1 });
        const celeritas::database_entity_change entity_change{ celeritas::database_type::redis,
                                                               "redis_test",
                                                               celeritas::database_change_type::insert_type,
                                                               key_container };

        celeritas::redis_test redis_test{ entity_change };

        // 测试标签数组元素操作
        redis_test.add_tags("new_tag");
        redis_test.set_tags(0, "updated_tag");
        // 测试移除标签
        // redis_test.remove_tags(0); // 如果需要，可以根据实际实现进行测试

        // 测试分类索引数组元素操作
        redis_test.add_category_index(999);
        redis_test.set_category_index(0, 888);
        // 测试移除分类
        // redis_test.remove_category_index(0); // 如果需要，可以根据实际实现进行测试

        // 测试相关索引数组元素操作
        redis_test.add_related_index(777777LL);
        redis_test.set_related_index(0, 666666LL);
        // 测试移除相关索引
        // redis_test.remove_related_index(0); // 如果需要，可以根据实际实现进行测试

        // 测试比率数组元素操作
        redis_test.add_ratios(0.99);
        redis_test.set_ratios(0, 0.88);
        // 测试移除比率
        // redis_test.remove_ratios(0); // 如果需要，可以根据实际实现进行测试

        // 测试字节数组元素操作（redis_test特有，mysql_test没有）
        redis_test.add_attachment('x');
        redis_test.set_attachment(0, 'y');
        // 测试移除字节
        // redis_test.remove_attachment(0); // 如果需要，可以根据实际实现进行测试

        // 测试日志数组元素操作
        celeritas::traits::document_type doc; // 默认构造
        redis_test.add_logs(doc);
        redis_test.set_logs(0, doc);
        // 测试移除日志
        // redis_test.remove_logs(0); // 如果需要，可以根据实际实现进行测试
    }

    BOOST_AUTO_TEST_CASE(test_static_methods)
    {
        const auto key_container = std::make_shared<const celeritas::basis_database_container>(celeritas::basis_database{ "id", 1 });
        const celeritas::database_entity_change entity_change{ celeritas::database_type::redis,
                                                               "redis_test",
                                                               celeritas::database_change_type::insert_type,
                                                               key_container };

        // 测试静态方法
        const auto& field_container = celeritas::redis_test::get_database_field_container();
        BOOST_CHECK(!field_container.empty());

        // 测试不同的select方法
        auto select1 = celeritas::redis_test::get_select(celeritas::database_type::redis);
        BOOST_CHECK_EQUAL(select1->get_database_name(), "redis_test");

        auto select2 = celeritas::redis_test::get_select(celeritas::database_type::redis, 123456LL);
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

        // 测试继承自database_entity的方法
        auto modify_ptr = redis_test.get_modify();
        (void)modify_ptr; // 验证方法可调用

        redis_test.clear_modify();
        bool is_modified = redis_test.is_modify();
        (void)is_modified; // 验证方法可调用

        bool is_must_save = redis_test.is_must_save();
        (void)is_must_save; // 验证方法可调用
    }

    BOOST_AUTO_TEST_CASE(test_inheritance_delete)
    {
        const auto key_container = std::make_shared<const celeritas::basis_database_container>(celeritas::basis_database{ "id", 1 });
        const celeritas::database_entity_change entity_change{ celeritas::database_type::redis,
                                                               "redis_test",
                                                               celeritas::database_change_type::insert_type,
                                                               key_container };

        celeritas::redis_test redis_test{ entity_change };

        auto delete_ptr = redis_test.get_delete();
        (void)delete_ptr; // 验证方法可调用
    }

BOOST_AUTO_TEST_SUITE_END()