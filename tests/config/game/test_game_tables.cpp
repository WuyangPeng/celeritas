#include "common/core/celeritas_error.h"
#include "config/game/game_tables.h"
#include "config/luban/generated/schema.h"

#include <boost/test/unit_test.hpp>
#include <memory>
#include <vector>

// 辅助函数：创建一个包含测试数据的 config::tables
std::shared_ptr<const celeritas::config::tables> create_test_tables()
{
    // 构造一个 ByteBuf 来模拟序列化的数据
    // 注意：这需要了解 Luban 的序列化格式。
    // 根据 schema.h，tables::load 调用了各个 container 的 load。
    // 每个 container load 时先读取大小 n，然后读取 n 个元素。
    // 元素的 deserialize 方法会读取各个字段。

    // 为了简化，我们只填充 surname_config_container 和 name_config_container
    // 其他容器填充 0 大小。

    luban::ByteBuf buf;

    // 1. surname_config_container
    // 写入大小 2
    buf.writeSize(2);

    // surname 1: id=1, name="Zhao", weight=10
    buf.writeInt(1); // id
    buf.writeString("Zhao"); // name
    buf.writeInt(10); // weight

    // surname 2: id=2, name="Qian", weight=20
    buf.writeInt(2); // id
    buf.writeString("Qian"); // name
    buf.writeInt(20); // weight

    // 2. name_config_container
    // 写入大小 3
    buf.writeSize(3);

    // name 1: id=1, name="MaleName", sexType=male(1), weight=10
    buf.writeInt(1); // id
    buf.writeString("MaleName"); // name
    buf.writeInt(1); // sexType (male)
    buf.writeInt(10); // weight

    // name 2: id=2, name="FemaleName", sexType=female(2), weight=10
    buf.writeInt(2); // id
    buf.writeString("FemaleName"); // name
    buf.writeInt(2); // sexType (female)
    buf.writeInt(10); // weight

    // name 3: id=3, name="AnyName", sexType=none(0), weight=10
    buf.writeInt(3); // id
    buf.writeString("AnyName"); // name
    buf.writeInt(0); // sexType (none)
    buf.writeInt(10); // weight

    // 3. red_dot_config_container (empty)
    buf.writeSize(0);

    // 4. item_config_container (empty)
    buf.writeSize(0);

    // 5. develop_config_container (empty)
    buf.writeSize(0);

    // 6. rename_cost_config_container (empty)
    buf.writeSize(0);

    // 创建 tables 对象并加载
    auto tables = std::make_shared<celeritas::config::tables>();

    // 模拟 loader
    auto loader = [&](luban::ByteBuf& out_buf, const std::string& name) -> bool {
        // 简单起见，我们忽略 name，假设所有数据都在 buf 中按顺序排列
        // 但实际上 tables::load 会多次调用 loader，每次针对不同的 container。
        // 所以我们需要把 buf 分割或者根据 name 返回对应的数据。

        // 重新审视 tables::load 实现：
        // if (!loader(buf, "surname_config_container")) return false;
        // if (!surname_config_container.load(buf)) return false;
        // ...

        // 这意味着我们需要为每个 container 准备单独的 ByteBuf 数据。

        out_buf.clear();

        if (name == "surname_config_container")
        {
            out_buf.writeSize(2);
            out_buf.writeInt(1);
            out_buf.writeString("Zhao");
            out_buf.writeInt(10);
            out_buf.writeInt(2);
            out_buf.writeString("Qian");
            out_buf.writeInt(20);
        }
        else if (name == "name_config_container")
        {
            out_buf.writeSize(3);
            out_buf.writeInt(1);
            out_buf.writeString("MaleName");
            out_buf.writeInt(1);
            out_buf.writeInt(10);
            out_buf.writeInt(2);
            out_buf.writeString("FemaleName");
            out_buf.writeInt(2);
            out_buf.writeInt(10);
            out_buf.writeInt(3);
            out_buf.writeString("AnyName");
            out_buf.writeInt(0);
            out_buf.writeInt(10);
        }
        else
        {
            out_buf.writeSize(0);
        }
        return true;
    };

    if (!tables->load(loader))
    {
        throw std::runtime_error("Failed to load mock tables");
    }

    return tables;
}

BOOST_AUTO_TEST_SUITE(game_tables_suite)

    BOOST_AUTO_TEST_CASE(test_game_tables_init_and_get_surname)
    {
        auto tables_data = create_test_tables();
        celeritas::game_tables game_tables(tables_data);

        // 测试 get_tables
        BOOST_CHECK(game_tables.get_tables() == tables_data);

        // 测试 get_surname
        // 由于是随机的，我们多次调用，确保返回的是配置中的名字
        for (int i = 0; i < 10; ++i)
        {
            std::string surname = game_tables.get_surname();
            BOOST_CHECK(surname == "Zhao" || surname == "Qian");
        }
    }

    BOOST_AUTO_TEST_CASE(test_game_tables_get_name)
    {
        auto tables_data = create_test_tables();
        celeritas::game_tables game_tables(tables_data);

        // 测试男性名字 (MaleName 或 AnyName)
        // 注意：init_name_config 逻辑：
        // male_name_weight 包含 sexType != female 的名字 (即 male 和 none)
        for (int i = 0; i < 10; ++i)
        {
            std::string name = game_tables.get_name(celeritas::game_tables::sex_type::male);
            BOOST_CHECK(name == "MaleName" || name == "AnyName");
        }

        // 测试女性名字 (FemaleName 或 AnyName)
        // female_name_weight 包含 sexType != male 的名字 (即 female 和 none)
        for (int i = 0; i < 10; ++i)
        {
            std::string name = game_tables.get_name(celeritas::game_tables::sex_type::female);
            BOOST_CHECK(name == "FemaleName" || name == "AnyName");
        }

        // 测试无性别名字 (AnyName, MaleName, FemaleName 都会包含在 null_name_weight 中)
        // null_name_weight 包含所有名字
        for (int i = 0; i < 10; ++i)
        {
            std::string name = game_tables.get_name(celeritas::game_tables::sex_type::none);
            BOOST_CHECK(name == "MaleName" || name == "FemaleName" || name == "AnyName");
        }
    }

    BOOST_AUTO_TEST_CASE(test_game_tables_invalid_sex_type)
    {
        auto tables_data = create_test_tables();
        celeritas::game_tables game_tables(tables_data);

        // 应该抛出异常，因为 map 中找不到对应的 key
        BOOST_CHECK_THROW([game_tables] { std::ignore = game_tables.get_name(static_cast<celeritas::game_tables::sex_type>(999));}(), celeritas::celeritas_error);
    }

BOOST_AUTO_TEST_SUITE_END()
