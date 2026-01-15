#include "database/document/consumable_data.h"
#include "database/basic/basis_database.h"

#include <boost/test/unit_test.hpp>

#include <string>
#include <vector>

BOOST_AUTO_TEST_SUITE(consumable_data_suite)

    BOOST_AUTO_TEST_CASE(test_default_constructor)
    {
        celeritas::consumable_data data;

        // 验证默认构造函数初始化的值
        BOOST_CHECK_EQUAL(data.get_expire_time(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_parameterized_constructor)
    {
        constexpr int64_t test_expire_time = 123456789LL;
        celeritas::consumable_data data(test_expire_time);

        // 验证参数化构造函数设置的值
        BOOST_CHECK_EQUAL(data.get_expire_time(), test_expire_time);
    }

    BOOST_AUTO_TEST_CASE(test_expire_time_accessors)
    {
        celeritas::consumable_data data;
        constexpr int64_t test_value = 987654321LL;

        // 设置值
        data.set_expire_time(test_value);

        // 验证获取的值
        BOOST_CHECK_EQUAL(data.get_expire_time(), test_value);
    }

    // 测试to_document_type方法
    BOOST_AUTO_TEST_CASE(test_to_document_type)
    {
        constexpr int64_t test_expire_time = 123456LL;
        celeritas::consumable_data data(test_expire_time);

        // 调用转换方法
        const auto document = data.to_document_type();

        // 验证返回的文档类型包含正确的字段
        bool found_expire_time = false;
        for (const auto& element : document)
        {
            if (element.get_field_name() == celeritas::consumable_data::expire_time_description)
            {
                BOOST_CHECK_EQUAL(element.get_value<celeritas::database_data_type::int64_type>(), test_expire_time);
                found_expire_time = true;
                break;
            }
        }
        BOOST_CHECK_EQUAL(found_expire_time, true);
    }

    // 测试from_document静态方法
    BOOST_AUTO_TEST_CASE(test_from_document)
    {
        // 创建一个包含expire_time字段的文档
        celeritas::consumable_data::document_type doc{};
        constexpr int64_t test_expire_time = 999888777LL;
        doc.emplace_back(celeritas::consumable_data::expire_time_description, test_expire_time);

        // 调用静态方法
        const auto data = celeritas::consumable_data::from_document(doc);

        // 验证返回的对象包含正确的值
        BOOST_CHECK_EQUAL(data.get_expire_time(), test_expire_time);
    }

    // 测试from_document方法处理空文档的情况
    BOOST_AUTO_TEST_CASE(test_from_document_empty)
    {
        // 创建一个空文档
        celeritas::consumable_data::document_type doc{};

        // 调用静态方法
        const auto data = celeritas::consumable_data::from_document(doc);

        // 验证返回的对象使用默认值
        BOOST_CHECK_EQUAL(data.get_expire_time(), 0);
    }

    // 测试完整的数据流：设置值 -> 转换为文档 -> 从文档恢复
    BOOST_AUTO_TEST_CASE(test_complete_data_flow)
    {
        constexpr int64_t original_expire_time = 111222333LL;
        celeritas::consumable_data original_data(original_expire_time);

        // 将对象转换为文档
        const auto document = original_data.to_document_type();

        // 从文档创建新对象
        const auto restored_data = celeritas::consumable_data::from_document(document);

        // 验证值被正确保存和恢复
        BOOST_CHECK_EQUAL(restored_data.get_expire_time(), original_expire_time);
    }

BOOST_AUTO_TEST_SUITE_END()