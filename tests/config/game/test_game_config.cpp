#include "config/game/game_config.h"
#include "config/game/game_tables.h"
#include "config/luban/generated/schema.h"

#include <boost/test/unit_test.hpp>
#include <memory>

namespace
{
    [[nodiscard]] std::shared_ptr<const celeritas::config::tables> create_empty_tables()
    {
        auto tables = std::make_shared<celeritas::config::tables>();
        auto loader = [](luban::ByteBuf& buf, const std::string& name) -> bool {
            buf.writeSize(0);
            return true;
        };
        tables->load(loader);
        return tables;
    }
}

BOOST_AUTO_TEST_SUITE(game_config_suite)

    BOOST_AUTO_TEST_CASE(test_singleton_instance)
    {
        auto& instance1 = celeritas::game_config::get_instance();
        auto& instance2 = celeritas::game_config::get_instance();

        BOOST_CHECK_EQUAL(&instance1, &instance2);
    }

    BOOST_AUTO_TEST_CASE(test_set_and_get_game_tables)
    {
        auto& config = celeritas::game_config::get_instance();

        config.set_game_tables(nullptr);
        BOOST_CHECK(config.get_game_tables() == nullptr);

        auto empty_raw_tables = create_empty_tables();
        const auto game_tables_instance = std::make_shared<const celeritas::game_tables>(empty_raw_tables);

        config.set_game_tables(game_tables_instance);
        const auto retrieved_tables = config.get_game_tables();

        BOOST_CHECK(retrieved_tables != nullptr);
        BOOST_CHECK(retrieved_tables == game_tables_instance);
        BOOST_CHECK(retrieved_tables->get_tables() == empty_raw_tables);
    }

BOOST_AUTO_TEST_SUITE_END()
