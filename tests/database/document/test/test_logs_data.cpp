#include "database/document/test/logs_data.h"
#include "database/basic/basis_database.h"

#include <boost/test/unit_test.hpp>

#include <string>
#include <vector>

BOOST_AUTO_TEST_SUITE(logs_data_suite)

    BOOST_AUTO_TEST_CASE(test_default_constructor)
    {
        celeritas::logs_data data;

        // 验证默认构造函数初始化的值
        BOOST_CHECK_EQUAL(data.get_int64_value(), 0);
        BOOST_CHECK_EQUAL(data.get_string_value(), "");
        BOOST_CHECK_EQUAL(data.get_int32_value(), 0);
        BOOST_CHECK_EQUAL(data.get_int32_count_value(), 0);
        BOOST_CHECK_EQUAL(data.get_int64_count_value(), 0);
        BOOST_CHECK_EQUAL(data.get_double_value(), 0.0);
        BOOST_CHECK_EQUAL(data.get_bool_value(), false);
    }

    BOOST_AUTO_TEST_CASE(test_int64_value_accessors)
    {
        celeritas::logs_data data;
        constexpr int64_t test_value = 123456789LL;

        // 设置值
        data.set_int64_value(test_value);

        // 验证获取的值
        BOOST_CHECK_EQUAL(data.get_int64_value(), test_value);
    }

    BOOST_AUTO_TEST_CASE(test_string_value_accessors)
    {
        celeritas::logs_data data;
        const std::string test_value = "test_string_value";

        // 设置值
        data.set_string_value(test_value);

        // 验证获取的值
        BOOST_CHECK_EQUAL(data.get_string_value(), test_value);
    }

    BOOST_AUTO_TEST_CASE(test_string_array_value_accessors)
    {
        celeritas::logs_data data;
        const celeritas::logs_data::string_array_type test_value1{ "item1", "item2", "item3" };

        // 设置值
        data.set_string_array_value(test_value1);

        // 验证获取的值
        const auto& retrieved_value = data.get_string_array_value();
        BOOST_CHECK_EQUAL(retrieved_value.size(), 3);
        BOOST_CHECK_EQUAL(retrieved_value[0], "item1");
        BOOST_CHECK_EQUAL(retrieved_value[1], "item2");
        BOOST_CHECK_EQUAL(retrieved_value[2], "item3");
    }

    BOOST_AUTO_TEST_CASE(test_int32_value_accessors)
    {
        celeritas::logs_data data;
        constexpr int32_t test_value = 987654;

        // 设置值
        data.set_int32_value(test_value);

        // 验证获取的值
        BOOST_CHECK_EQUAL(data.get_int32_value(), test_value);
    }

    BOOST_AUTO_TEST_CASE(test_int32_count_value_accessors)
    {
        celeritas::logs_data data;
        constexpr int32_t test_value = 500;

        // 设置值
        data.set_int32_count_value(test_value);

        // 验证获取的值
        BOOST_CHECK_EQUAL(data.get_int32_count_value(), test_value);
    }

    BOOST_AUTO_TEST_CASE(test_int32_array_value_accessors)
    {
        celeritas::logs_data data;
        const celeritas::logs_data::int32_array_type test_value = { 10, 20, 30, 40 };

        // 设置值
        data.set_int32_array_value(test_value);

        // 验证获取的值
        const auto& retrieved_value = data.get_int32_array_value();
        BOOST_CHECK_EQUAL(retrieved_value.size(), 4);
        BOOST_CHECK_EQUAL(retrieved_value[0], 10);
        BOOST_CHECK_EQUAL(retrieved_value[1], 20);
        BOOST_CHECK_EQUAL(retrieved_value[2], 30);
        BOOST_CHECK_EQUAL(retrieved_value[3], 40);
    }

    BOOST_AUTO_TEST_CASE(test_int64_count_value_accessors)
    {
        celeritas::logs_data data;
        constexpr int64_t test_value = 999888777LL;

        // 设置值
        data.set_int64_count_value(test_value);

        // 验证获取的值
        BOOST_CHECK_EQUAL(data.get_int64_count_value(), test_value);
    }

    BOOST_AUTO_TEST_CASE(test_int64_array_value_accessors)
    {
        celeritas::logs_data data;
        const celeritas::logs_data::int64_array_type test_value = { 100LL, 200LL, 300LL };

        // 设置值
        data.set_int64_array_value(test_value);

        // 验证获取的值
        const auto& retrieved_value = data.get_int64_array_value();
        BOOST_CHECK_EQUAL(retrieved_value.size(), 3);
        BOOST_CHECK_EQUAL(retrieved_value[0], 100LL);
        BOOST_CHECK_EQUAL(retrieved_value[1], 200LL);
        BOOST_CHECK_EQUAL(retrieved_value[2], 300LL);
    }

    BOOST_AUTO_TEST_CASE(test_double_value_accessors)
    {
        celeritas::logs_data data;
        constexpr double test_value = 3.14159;

        // 设置值
        data.set_double_value(test_value);

        // 验证获取的值
        BOOST_CHECK_CLOSE(data.get_double_value(), test_value, 0.001);
    }

    BOOST_AUTO_TEST_CASE(test_double_array_value_accessors)
    {
        celeritas::logs_data data;
        const celeritas::logs_data::double_array_type test_value = { 1.1, 2.2, 3.3 };

        // 设置值
        data.set_double_array_value(test_value);

        // 验证获取的值
        const auto& retrieved_value = data.get_double_array_value();
        BOOST_CHECK_EQUAL(retrieved_value.size(), 3);
        BOOST_CHECK_CLOSE(retrieved_value[0], 1.1, 0.001);
        BOOST_CHECK_CLOSE(retrieved_value[1], 2.2, 0.001);
        BOOST_CHECK_CLOSE(retrieved_value[2], 3.3, 0.001);
    }

    BOOST_AUTO_TEST_CASE(test_bool_value_accessors)
    {
        celeritas::logs_data data;

        // 测试true值
        data.set_bool_value(true);
        BOOST_CHECK_EQUAL(data.get_bool_value(), true);

        // 测试false值
        data.set_bool_value(false);
        BOOST_CHECK_EQUAL(data.get_bool_value(), false);
    }

    BOOST_AUTO_TEST_CASE(test_document_value_accessors)
    {
        celeritas::logs_data data;
        // 创建一个简单的文档类型值（通常为bson::document::view_or_value或其他类似类型）
        celeritas::logs_data::document_type test_value; // 默认构造

        // 设置值
        data.set_document_value(test_value);

        // 验证获取的值
        const auto& retrieved_value = data.get_document_value();
        // 由于文档类型的具体实现未知，我们仅验证可以正常访问
        (void)retrieved_value; // 避免未使用警告
    }

    BOOST_AUTO_TEST_CASE(test_document_array_value_accessors)
    {
        celeritas::logs_data data;
        const celeritas::logs_data::document_array_type test_value = {}; // 空数组作为测试

        // 设置值
        data.set_document_array_value(test_value);

        // 验证获取的值
        const auto& retrieved_value = data.get_document_array_value();
        BOOST_CHECK_EQUAL(retrieved_value.size(), 0);
    }

    // 测试to_document_type方法
    BOOST_AUTO_TEST_CASE(test_to_document_type)
    {
        celeritas::logs_data data;

        // 设置一些测试数据
        data.set_int64_value(123456LL);
        data.set_string_value("test_string");
        data.set_bool_value(true);
        data.set_double_value(2.718);

        // 调用转换方法
        const auto document = data.to_document_type();

        // 验证返回的文档类型有效
        // 注意：具体的验证取决于document_type的实际类型和接口
        (void)document; // 避免未使用警告
    }

    // 测试from_document静态方法
    BOOST_AUTO_TEST_CASE(test_from_document)
    {
        // 创建一个文档
        celeritas::logs_data::document_type doc; // 默认构造

        // 调用静态方法
        const auto data = celeritas::logs_data::from_document(doc);

        // 验证返回的对象是有效的
        // 由于我们使用的是默认构造的文档，所以值应该是默认值
        BOOST_CHECK_EQUAL(data.get_int64_value(), 0);
        BOOST_CHECK_EQUAL(data.get_string_value(), "");
        BOOST_CHECK_EQUAL(data.get_bool_value(), false);
    }

    // 测试所有字段的一致性
    BOOST_AUTO_TEST_CASE(test_complete_data_flow)
    {
        celeritas::logs_data data;

        // 设置所有字段的值
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
        // data.set_document_value(...); // 由于文档类型复杂，跳过此字段
        // data.set_document_array_value(...); // 由于文档数组复杂，跳过此字段

        // 验证所有字段的值
        BOOST_CHECK_EQUAL(data.get_int64_value(), 111111LL);
        BOOST_CHECK_EQUAL(data.get_string_value(), "complete_test_string");
        BOOST_CHECK_EQUAL(data.get_string_array_value().size(), 3);
        BOOST_CHECK_EQUAL(data.get_int32_value(), 222);
        BOOST_CHECK_EQUAL(data.get_int32_count_value(), 333);
        BOOST_CHECK_EQUAL(data.get_int32_array_value().size(), 3);
        BOOST_CHECK_EQUAL(data.get_int64_count_value(), 777777LL);
        BOOST_CHECK_EQUAL(data.get_int64_array_value().size(), 2);
        BOOST_CHECK_CLOSE(data.get_double_value(), 1.2345, 0.001);
        BOOST_CHECK_EQUAL(data.get_double_array_value().size(), 3);
        BOOST_CHECK_EQUAL(data.get_bool_value(), true);
    }

BOOST_AUTO_TEST_SUITE_END()