#include "common/core/celeritas_error.h"
#include "config/game/game_tables.h"
#include "config/luban/generated/schema.h"

#include <boost/test/unit_test.hpp>

#include <memory>

namespace
{
    void write_surname_config(luban::ByteBuf& out_buf)
    {
        // 写入大小 2
        out_buf.writeSize(2);

        // surname 1: id=1, name="Zhao", weight=10
        out_buf.writeInt(1);
        out_buf.writeString("Zhao");
        out_buf.writeInt(10);

        // surname 2: id=2, name="Qian", weight=20
        out_buf.writeInt(2);
        out_buf.writeString("Qian");
        out_buf.writeInt(20);
    }

    void write_name_config(luban::ByteBuf& out_buf)
    {
        // 写入大小 3
        out_buf.writeSize(3);

        // name 1: id=1, name="MaleName", sexType=male(1), weight=10
        out_buf.writeInt(1);
        out_buf.writeString("MaleName");
        out_buf.writeInt(1);
        out_buf.writeInt(10);

        // name 2: id=2, name="FemaleName", sexType=female(2), weight=10
        out_buf.writeInt(2);
        out_buf.writeString("FemaleName");
        out_buf.writeInt(2);
        out_buf.writeInt(10);

        // name 3: id=3, name="AnyName", sexType=none(0), weight=10
        out_buf.writeInt(3);
        out_buf.writeString("AnyName");
        out_buf.writeInt(0);
        out_buf.writeInt(10);
    }

    [[nodiscard]] std::shared_ptr<const celeritas::config::tables> create_test_tables()
    {
        // 创建 tables 对象并加载
        auto tables = std::make_shared<celeritas::config::tables>();

        // 模拟 loader
        auto loader = [&](luban::ByteBuf& out_buf, const std::string& name) -> bool {
            out_buf.clear();

            if (name == "surname_config_container")
            {
                write_surname_config(out_buf);
            }
            else if (name == "name_config_container")
            {
                write_name_config(out_buf);
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
}

BOOST_AUTO_TEST_SUITE(game_tables_suite)

    BOOST_AUTO_TEST_CASE(test_game_tables_init)
    {
        const auto tables_data = create_test_tables();
        const celeritas::game_tables game_tables{ tables_data };

        BOOST_CHECK(game_tables.get_tables() == tables_data);
    }

BOOST_AUTO_TEST_SUITE_END()
