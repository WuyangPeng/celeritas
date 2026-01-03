#include "config/game/game_config.h"
#include "config/game/game_tables.h"

#include <boost/test/unit_test.hpp>

// 由于 game_config 是单例且涉及复杂的 Luban 生成代码，
// 这里的测试主要验证单例获取和基本的指针设置/获取。
// 真正的加载逻辑 (load_tables) 依赖于外部数据文件和生成的代码，
// 在单元测试中很难完全模拟，除非 mock 整个文件系统和生成代码。

BOOST_AUTO_TEST_SUITE(game_config_suite)

    BOOST_AUTO_TEST_CASE(test_singleton_instance)
    {
        auto& instance1 = celeritas::game_config::get_instance();
        auto& instance2 = celeritas::game_config::get_instance();

        BOOST_CHECK_EQUAL(&instance1, &instance2);
    }

    BOOST_AUTO_TEST_CASE(test_set_get_game_tables)
    {
        auto& config = celeritas::game_config::get_instance();

        // 初始状态可能是 nullptr，或者之前的测试已经设置过。
        // 我们创建一个空的 game_tables (如果构造函数允许) 或者 mock 一个。
        // game_tables 的构造函数需要 std::shared_ptr<const config::tables>。
        // config::tables 是 Luban 生成的类。

        // 由于我们无法轻易构造 config::tables (它可能很大且依赖数据加载)，
        // 我们这里测试 set/get 机制本身，传递 nullptr 是最简单的验证方式，
        // 虽然在实际逻辑中不应该为 null。

        std::shared_ptr<const celeritas::game_tables> null_tables = nullptr;
        config.set_game_tables(null_tables);
        BOOST_CHECK(config.get_game_tables() == nullptr);

        // 如果能构造一个 dummy tables 最好，但依赖 generated schema。
    }

BOOST_AUTO_TEST_SUITE_END()
