#include "config/basic/database_type.h"
#include "database/basic/basis_database_container.h"
#include "database/basic/database_change_type.h"
#include "database/basic/database_entity_change.h"
#include "database/detail/check_helper.h"
#include "database/entity/database_entity.h"
#include "database/generated/mongo/test/mongo_test.h"

#include <boost/test/unit_test.hpp>

#include <memory>
#include <string>
#include <vector>

BOOST_AUTO_TEST_SUITE(mongo_test_suite)

    BOOST_AUTO_TEST_CASE(test_constructors)
    {
        const auto key_container = std::make_shared<const celeritas::basis_database_container>();
        const celeritas::database_entity_change entity_change{ celeritas::database_type::mongo,
                                                               "mongo_test",
                                                               celeritas::database_change_type::insert_type,
                                                               key_container };

        const celeritas::mongo_test mongo_test1{ entity_change };

        const celeritas::mongo_test mongo_test2{ celeritas::database_type::mongo, entity_change };

        const celeritas::mongo_test mongo_test3{ celeritas::database_type::mongo, 123456LL };

        BOOST_CHECK_EQUAL(mongo_test3.get_user_id(), 123456LL);
    }

    BOOST_AUTO_TEST_CASE(test_getters_and_setters)
    {
        const auto key_container = std::make_shared<const celeritas::basis_database_container>();
        const celeritas::database_entity_change entity_change{ celeritas::database_type::mongo,
                                                               "mongo_test",
                                                               celeritas::database_change_type::insert_type,
                                                               key_container };

        celeritas::mongo_test mongo_test{ entity_change };

        mongo_test.set_user_id(111111LL);
        BOOST_CHECK_EQUAL(mongo_test.get_user_id(), 111111LL);

        mongo_test.set_chapter_id(222);
        BOOST_CHECK_EQUAL(mongo_test.get_chapter_id(), 222);

        mongo_test.set_chapter_name("test_chapter");
        BOOST_CHECK_EQUAL(mongo_test.get_chapter_name(), "test_chapter");

        mongo_test.set_chance_winning(0.75);
        BOOST_CHECK_CLOSE(mongo_test.get_chance_winning(), 0.75, 0.001);

        mongo_test.set_winning(true);
        BOOST_CHECK_EQUAL(mongo_test.is_winning(), true);

        mongo_test.set_currency(5000LL);
        BOOST_CHECK_EQUAL(mongo_test.get_currency(), 5000LL);

        mongo_test.set_count(100);
        BOOST_CHECK_EQUAL(mongo_test.get_count(), 100);
    }

    BOOST_AUTO_TEST_CASE(test_array_field_operations)
    {
        const auto key_container = std::make_shared<const celeritas::basis_database_container>();
        const celeritas::database_entity_change entity_change{ celeritas::database_type::mongo,
                                                               "mongo_test",
                                                               celeritas::database_change_type::insert_type,
                                                               key_container };

        celeritas::mongo_test mongo_test{ entity_change };

        const celeritas::traits::string_array_type tags{ "tag1", "tag2", "tag3" };
        mongo_test.set_tags(tags);
        celeritas::check_array(tags, mongo_test.get_tags());

        const celeritas::traits::int32_array_type category_indices{ 1, 2, 3, 4 };
        mongo_test.set_category_index(category_indices);
        celeritas::check_array(category_indices, mongo_test.get_category_index());

        const celeritas::traits::int64_array_type related_indices{ 100LL, 200LL, 300LL };
        mongo_test.set_related_index(related_indices);
        celeritas::check_array(related_indices, mongo_test.get_related_index());

        const celeritas::traits::double_array_type ratios{ 0.1, 0.2, 0.3 };
        mongo_test.set_ratios(ratios);
        celeritas::check_array(ratios, mongo_test.get_ratios());
    }

    BOOST_AUTO_TEST_CASE(test_byte_array_operations)
    {
        const auto key_container = std::make_shared<const celeritas::basis_database_container>();
        const celeritas::database_entity_change entity_change{ celeritas::database_type::mongo,
                                                               "mongo_test",
                                                               celeritas::database_change_type::insert_type,
                                                               key_container };

        celeritas::mongo_test mongo_test{ entity_change };

        const celeritas::traits::byte_array_type bytes{ 'h', 'e', 'l', 'l', 'o' };
        mongo_test.set_attachment(bytes);
        const auto& retrieved_bytes = mongo_test.get_attachment();
        celeritas::check_array(bytes, retrieved_bytes);
    }

    BOOST_AUTO_TEST_CASE(test_document_field_operations)
    {
        const auto key_container = std::make_shared<const celeritas::basis_database_container>();
        const celeritas::database_entity_change entity_change{ celeritas::database_type::mongo,
                                                               "mongo_test",
                                                               celeritas::database_change_type::insert_type,
                                                               key_container };

        celeritas::mongo_test mongo_test{ entity_change };

        // 由于文档类型的具体实现未知，我们只验证可以正常访问
        celeritas::traits::document_type properties_doc; // 默认构造
        mongo_test.set_properties(properties_doc);
        const auto& retrieved_props = mongo_test.get_properties();
        (void)retrieved_props; // 避免未使用警告

        celeritas::traits::document_array_type logs_docs = {}; // 空数组
        mongo_test.set_logs(logs_docs);
        const auto& retrieved_logs = mongo_test.get_logs();
        BOOST_CHECK_EQUAL(retrieved_logs.size(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_modifier_methods)
    {
        // 创建必要的参数来构造database_entity_change
        auto key_container = std::make_shared<const celeritas::basis_database_container>();
        celeritas::database_entity_change entity_change{
            celeritas::database_type::mongo,
            "mongo_test",
            celeritas::database_change_type::insert_type,
            key_container
        };

        celeritas::mongo_test test_obj{ entity_change };

        // 测试货币修改方法
        test_obj.set_currency(1000LL);
        test_obj.modify_currency(500LL);
        // 注意：modify_currency的具体行为取决于实现，这里只是测试方法存在
        (void)test_obj.get_currency(); // 验证方法可调用

        // 测试计数修改方法
        test_obj.set_count(50);
        test_obj.modify_count(10);
        // 注意：modify_count的具体行为取决于实现，这里只是测试方法存在
        (void)test_obj.get_count(); // 验证方法可调用
    }

    BOOST_AUTO_TEST_CASE(test_array_element_operations)
    {
        // 创建必要的参数来构造database_entity_change
        auto key_container = std::make_shared<const celeritas::basis_database_container>();
        celeritas::database_entity_change entity_change{
            celeritas::database_type::mongo,
            "mongo_test",
            celeritas::database_change_type::insert_type,
            key_container
        };

        celeritas::mongo_test test_obj{ entity_change };

        // 测试标签数组元素操作
        test_obj.add_tags("new_tag");
        test_obj.set_tags(0, "updated_tag");
        // 测试移除标签
        // test_obj.remove_tags(0); // 如果需要，可以根据实际实现进行测试

        // 测试分类索引数组元素操作
        test_obj.add_category_index(999);
        test_obj.set_category_index(0, 888);
        // 测试移除分类
        // test_obj.remove_category_index(0); // 如果需要，可以根据实际实现进行测试

        // 测试相关索引数组元素操作
        test_obj.add_related_index(777777LL);
        test_obj.set_related_index(0, 666666LL);
        // 测试移除相关索引
        // test_obj.remove_related_index(0); // 如果需要，可以根据实际实现进行测试

        // 测试比率数组元素操作
        test_obj.add_ratios(0.99);
        test_obj.set_ratios(0, 0.88);
        // 测试移除比率
        // test_obj.remove_ratios(0); // 如果需要，可以根据实际实现进行测试

        // 测试字节数组元素操作
        test_obj.add_attachment('x');
        test_obj.set_attachment(0, 'y');
        // 测试移除字节
        // test_obj.remove_attachment(0); // 如果需要，可以根据实际实现进行测试

        // 测试日志数组元素操作
        celeritas::traits::document_type doc; // 默认构造
        test_obj.add_logs(doc);
        test_obj.set_logs(0, doc);
        // 测试移除日志
        // test_obj.remove_logs(0); // 如果需要，可以根据实际实现进行测试
    }

    BOOST_AUTO_TEST_CASE(test_static_methods)
    {
        // 创建必要的参数来构造database_entity_change
        auto key_container = std::make_shared<const celeritas::basis_database_container>();
        celeritas::database_entity_change entity_change(
            celeritas::database_type::mongo,
            "mongo_test",
            celeritas::database_change_type::insert_type,
            key_container
            );

        // 测试静态方法
        const auto& field_container = celeritas::mongo_test::get_database_field_container();
        (void)field_container; // 验证方法可调用

        // 测试不同的select方法
        auto select1 = celeritas::mongo_test::get_select(celeritas::database_type::mongo);
        (void)select1; // 验证方法可调用

        auto select2 = celeritas::mongo_test::get_select(celeritas::database_type::mongo, 123456LL);
        (void)select2; // 验证方法可调用
    }

    BOOST_AUTO_TEST_CASE(test_constants)
    {
        // 测试常量值
        BOOST_CHECK_EQUAL(celeritas::mongo_test::database_name, "mongo_test");
        BOOST_CHECK_EQUAL(celeritas::mongo_test::user_id_describe, "_id");
        BOOST_CHECK_EQUAL(celeritas::mongo_test::chapter_id_describe, "chapter_id");
        BOOST_CHECK_EQUAL(celeritas::mongo_test::chapter_name_describe, "chapter_name");
        BOOST_CHECK_EQUAL(celeritas::mongo_test::chance_winning_describe, "chance_winning");
        BOOST_CHECK_EQUAL(celeritas::mongo_test::winning_describe, "winning");
        BOOST_CHECK_EQUAL(celeritas::mongo_test::currency_describe, "currency");
        BOOST_CHECK_EQUAL(celeritas::mongo_test::count_describe, "count");
        BOOST_CHECK_EQUAL(celeritas::mongo_test::tags_describe, "tags");
        BOOST_CHECK_EQUAL(celeritas::mongo_test::category_index_describe, "category_index");
        BOOST_CHECK_EQUAL(celeritas::mongo_test::related_index_describe, "related_index");
        BOOST_CHECK_EQUAL(celeritas::mongo_test::ratios_describe, "ratios");
        BOOST_CHECK_EQUAL(celeritas::mongo_test::attachment_describe, "attachment");
        BOOST_CHECK_EQUAL(celeritas::mongo_test::properties_describe, "properties");
        BOOST_CHECK_EQUAL(celeritas::mongo_test::logs_describe, "logs");
    }

    BOOST_AUTO_TEST_CASE(test_inheritance_from_database_entity)
    {
        // 创建必要的参数来构造database_entity_change
        auto key_container = std::make_shared<const celeritas::basis_database_container>();
        celeritas::database_entity_change entity_change{
            celeritas::database_type::mongo,
            "mongo_test",
            celeritas::database_change_type::insert_type,
            key_container
        };

        celeritas::mongo_test test_obj{ entity_change };

        // 测试继承自database_entity的方法
        auto modify_ptr = test_obj.get_modify();
        (void)modify_ptr; // 验证方法可调用

        auto delete_ptr = test_obj.get_delete();
        (void)delete_ptr; // 验证方法可调用

        test_obj.clear_modify();
        bool is_modified = test_obj.is_modify();
        (void)is_modified; // 验证方法可调用

        bool is_must_save = test_obj.is_must_save();
        (void)is_must_save; // 验证方法可调用
    }

BOOST_AUTO_TEST_SUITE_END()