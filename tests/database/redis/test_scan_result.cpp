#include "database/redis/scan_result.h"

#include <boost/test/unit_test.hpp>
#include <string>
#include <vector>

BOOST_AUTO_TEST_SUITE(scan_result_suite)

    BOOST_AUTO_TEST_CASE(test_scan_result_constructor_and_getters)
    {
        const std::string cursor{ "12345" };
        const celeritas::scan_result::array_type keys{ "key1", "key2", "key3" };

        const celeritas::scan_result result{ cursor, keys };

        // 验证获取游标的方法
        BOOST_CHECK_EQUAL(result.get_cursor(), cursor);

        // 验证获取键列表的方法
        const auto retrieved_keys = result.get_keys();
        BOOST_CHECK_EQUAL(retrieved_keys.size(), keys.size());
        for (size_t i = 0; i < keys.size(); ++i)
        {
            BOOST_CHECK_EQUAL(retrieved_keys[i], keys[i]);
        }
    }

    BOOST_AUTO_TEST_CASE(test_scan_result_with_empty_cursor)
    {
        // 测试空游标的场景
        const std::string cursor{};
        const celeritas::scan_result::array_type keys{ "key1", "key2" };

        const celeritas::scan_result result{ cursor, keys };

        BOOST_CHECK_EQUAL(result.get_cursor(), cursor);
        BOOST_CHECK_EQUAL(result.get_keys().size(), 2);
        BOOST_CHECK_EQUAL(result.get_keys()[0], "key1");
        BOOST_CHECK_EQUAL(result.get_keys()[1], "key2");
    }

    BOOST_AUTO_TEST_CASE(test_scan_result_with_empty_keys)
    {
        const std::string cursor{ "67890" };
        const celeritas::scan_result::array_type keys{};

        const celeritas::scan_result result{ cursor, keys };

        BOOST_CHECK_EQUAL(result.get_cursor(), cursor);
        BOOST_CHECK_EQUAL(result.get_keys().size(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_scan_result_with_both_empty)
    {
        // 测试游标和键列表都为空的场景
        const std::string cursor{};
        const celeritas::scan_result::array_type keys{};

        const celeritas::scan_result result{ cursor, keys };

        BOOST_CHECK_EQUAL(result.get_cursor(), "");
        BOOST_CHECK_EQUAL(result.get_keys().size(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_scan_result_move_semantics)
    {
        // 测试移动语义是否正确工作
        std::string cursor{ "move_test_cursor" };
        celeritas::scan_result::array_type keys{ "move_key1", "move_key2", "move_key3" };

        // 使用临时对象创建结果
        const celeritas::scan_result result{ "temp_cursor",
                                             celeritas::scan_result::array_type{ "temp_key1", "temp_key2" } };

        BOOST_CHECK_EQUAL(result.get_cursor(), "temp_cursor");
        const auto retrieved_keys = result.get_keys();
        BOOST_CHECK_EQUAL(retrieved_keys.size(), 2);
        BOOST_CHECK_EQUAL(retrieved_keys[0], "temp_key1");
        BOOST_CHECK_EQUAL(retrieved_keys[1], "temp_key2");
    }

BOOST_AUTO_TEST_SUITE_END()