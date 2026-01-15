#include "config/basic/database_type.h"
#include "database/generated/mysql/test/mysql_test.h"
#include "database/entity/database_entity.h"
#include "database/basic/basis_database.h"
#include "database/basic/database_data_type.h"
#include "database/basic/database_entity_change.h"
#include "database/basic/basis_database_container.h"
#include "database/basic/database_change_type.h"

#include <boost/test/unit_test.hpp>

#include <string>
#include <vector>
#include <memory>

BOOST_AUTO_TEST_SUITE(mysql_test_suite)

    BOOST_AUTO_TEST_CASE(test_constructors)
    {
        // 创建必要的参数来构造database_entity_change
        auto key_container = std::make_shared<const celeritas::basis_database_container>();
        celeritas::database_entity_change entity_change(
            celeritas::database_type::mysql,
            "mysql_test",
            celeritas::database_change_type::insert_type,
            key_container
            );

        // 测试不同的构造函数
        celeritas::mysql_test test_obj1(entity_change);

        // 测试带数据库类型参数的构造函数
        celeritas::mysql_test test_obj2(celeritas::database_type::mysql, entity_change);

        // 测试带用户ID参数的构造函数
        celeritas::mysql_test test_obj3(celeritas::database_type::mysql, 123456LL);

        // 验证对象被正确构造
        BOOST_CHECK_EQUAL(test_obj3.get_user_id(), 123456LL);
    }

    BOOST_AUTO_TEST_CASE(test_getters_and_setters)
    {
        // 创建必要的参数来构造database_entity_change
        auto key_container = std::make_shared<const celeritas::basis_database_container>();
        celeritas::database_entity_change entity_change(
            celeritas::database_type::mysql,
            "mysql_test",
            celeritas::database_change_type::insert_type,
            key_container
            );

        celeritas::mysql_test test_obj(entity_change);

        // 测试各种getter和setter方法
        test_obj.set_user_id(111111LL);
        BOOST_CHECK_EQUAL(test_obj.get_user_id(), 111111LL);

        test_obj.set_chapter_id(222);
        BOOST_CHECK_EQUAL(test_obj.get_chapter_id(), 222);

        test_obj.set_chapter_name("test_chapter");
        BOOST_CHECK_EQUAL(test_obj.get_chapter_name(), "test_chapter");

        test_obj.set_chance_winning(0.75);
        BOOST_CHECK_CLOSE(test_obj.get_chance_winning(), 0.75, 0.001);

        test_obj.set_winning(true);
        BOOST_CHECK_EQUAL(test_obj.is_winning(), true);

        test_obj.set_currency(5000LL);
        BOOST_CHECK_EQUAL(test_obj.get_currency(), 5000LL);

        test_obj.set_count(100);
        BOOST_CHECK_EQUAL(test_obj.get_count(), 100);
    }

    BOOST_AUTO_TEST_CASE(test_array_field_operations)
    {
        // 创建必要的参数来构造database_entity_change
        auto key_container = std::make_shared<const celeritas::basis_database_container>();
        celeritas::database_entity_change entity_change(
            celeritas::database_type::mysql,
            "mysql_test",
            celeritas::database_change_type::insert_type,
            key_container
            );

        celeritas::mysql_test test_obj(entity_change);

        // 测试标签数组
        celeritas::traits::string_array_type tags = { "tag1", "tag2", "tag3" };
        test_obj.set_tags(tags);
        const auto& retrieved_tags = test_obj.get_tags();
        BOOST_CHECK_EQUAL(retrieved_tags.size(), 3);
        BOOST_CHECK_EQUAL(retrieved_tags[0], "tag1");
        BOOST_CHECK_EQUAL(retrieved_tags[1], "tag2");
        BOOST_CHECK_EQUAL(retrieved_tags[2], "tag3");

        // 测试分类索引数组
        celeritas::traits::int32_array_type category_indices = { 1, 2, 3, 4 };
        test_obj.set_category_index(category_indices);
        const auto& retrieved_categories = test_obj.get_category_index();
        BOOST_CHECK_EQUAL(retrieved_categories.size(), 4);
        BOOST_CHECK_EQUAL(retrieved_categories[0], 1);
        BOOST_CHECK_EQUAL(retrieved_categories[1], 2);
        BOOST_CHECK_EQUAL(retrieved_categories[2], 3);
        BOOST_CHECK_EQUAL(retrieved_categories[3], 4);

        // 测试相关索引数组
        celeritas::traits::int64_array_type related_indices = { 100LL, 200LL, 300LL };
        test_obj.set_related_index(related_indices);
        const auto& retrieved_related = test_obj.get_related_index();
        BOOST_CHECK_EQUAL(retrieved_related.size(), 3);
        BOOST_CHECK_EQUAL(retrieved_related[0], 100LL);
        BOOST_CHECK_EQUAL(retrieved_related[1], 200LL);
        BOOST_CHECK_EQUAL(retrieved_related[2], 300LL);

        // 测试比率数组
        celeritas::traits::double_array_type ratios = { 0.1, 0.2, 0.3 };
        test_obj.set_ratios(ratios);
        const auto& retrieved_ratios = test_obj.get_ratios();
        BOOST_CHECK_EQUAL(retrieved_ratios.size(), 3);
        BOOST_CHECK_CLOSE(retrieved_ratios[0], 0.1, 0.001);
        BOOST_CHECK_CLOSE(retrieved_ratios[1], 0.2, 0.001);
        BOOST_CHECK_CLOSE(retrieved_ratios[2], 0.3, 0.001);
    }

    BOOST_AUTO_TEST_CASE(test_document_field_operations)
    {
        // 创建必要的参数来构造database_entity_change
        auto key_container = std::make_shared<const celeritas::basis_database_container>();
        celeritas::database_entity_change entity_change(
            celeritas::database_type::mysql,
            "mysql_test",
            celeritas::database_change_type::insert_type,
            key_container
            );

        celeritas::mysql_test test_obj(entity_change);

        // 由于文档类型的具体实现未知，我们只验证可以正常访问
        celeritas::traits::document_type properties_doc; // 默认构造
        test_obj.set_properties(properties_doc);
        const auto& retrieved_props = test_obj.get_properties();
        (void)retrieved_props; // 避免未使用警告

        celeritas::traits::document_array_type logs_docs = {}; // 空数组
        test_obj.set_logs(logs_docs);
        const auto& retrieved_logs = test_obj.get_logs();
        BOOST_CHECK_EQUAL(retrieved_logs.size(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_modifier_methods)
    {
        // 创建必要的参数来构造database_entity_change
        auto key_container = std::make_shared<const celeritas::basis_database_container>();
        celeritas::database_entity_change entity_change(
            celeritas::database_type::mysql,
            "mysql_test",
            celeritas::database_change_type::insert_type,
            key_container
            );

        celeritas::mysql_test test_obj(entity_change);

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
        celeritas::database_entity_change entity_change(
            celeritas::database_type::mysql,
            "mysql_test",
            celeritas::database_change_type::insert_type,
            key_container
            );

        celeritas::mysql_test test_obj(entity_change);

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
            celeritas::database_type::mysql,
            "mysql_test",
            celeritas::database_change_type::insert_type,
            key_container
            );

        // 测试静态方法
        const auto& field_container = celeritas::mysql_test::get_database_field_container();
        (void)field_container; // 验证方法可调用

        // 测试不同的select方法
        auto select1 = celeritas::mysql_test::get_select(celeritas::database_type::mysql);
        (void)select1; // 验证方法可调用

        auto select2 = celeritas::mysql_test::get_select(celeritas::database_type::mysql, 123456LL);
        (void)select2; // 验证方法可调用
    }

    BOOST_AUTO_TEST_CASE(test_constants)
    {
        // 测试常量值
        BOOST_CHECK_EQUAL(celeritas::mysql_test::database_name, "mysql_test");
        BOOST_CHECK_EQUAL(celeritas::mysql_test::user_id_describe, "user_id");
        BOOST_CHECK_EQUAL(celeritas::mysql_test::chapter_id_describe, "chapter_id");
        BOOST_CHECK_EQUAL(celeritas::mysql_test::chapter_name_describe, "chapter_name");
        BOOST_CHECK_EQUAL(celeritas::mysql_test::chance_winning_describe, "chance_winning");
        BOOST_CHECK_EQUAL(celeritas::mysql_test::winning_describe, "winning");
        BOOST_CHECK_EQUAL(celeritas::mysql_test::currency_describe, "currency");
        BOOST_CHECK_EQUAL(celeritas::mysql_test::count_describe, "count");
        BOOST_CHECK_EQUAL(celeritas::mysql_test::tags_describe, "tags");
        BOOST_CHECK_EQUAL(celeritas::mysql_test::category_index_describe, "category_index");
        BOOST_CHECK_EQUAL(celeritas::mysql_test::related_index_describe, "related_index");
        BOOST_CHECK_EQUAL(celeritas::mysql_test::ratios_describe, "ratios");
        BOOST_CHECK_EQUAL(celeritas::mysql_test::properties_describe, "properties");
        BOOST_CHECK_EQUAL(celeritas::mysql_test::logs_describe, "logs");
    }

    BOOST_AUTO_TEST_CASE(test_inheritance_from_database_entity)
    {
        // 创建必要的参数来构造database_entity_change
        auto key_container = std::make_shared<const celeritas::basis_database_container>();
        celeritas::database_entity_change entity_change(
            celeritas::database_type::mysql,
            "mysql_test",
            celeritas::database_change_type::insert_type,
            key_container
            );

        celeritas::mysql_test test_obj(entity_change);

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