#include "player/time/player_time_refresh_key.h"
#include "player/time/time_refresh_type.h"

#include <boost/test/unit_test.hpp>

#include <map>

BOOST_AUTO_TEST_SUITE(player_time_refresh_key_suite)

    BOOST_AUTO_TEST_CASE(test_full_constructor)
    {
        constexpr auto refresh_type = celeritas::time_refresh_type::daily;
        constexpr auto parameter = 12345LL;
        constexpr auto time_id = 67890LL;

        const celeritas::player_time_refresh_key key{ refresh_type, parameter, time_id };

        BOOST_CHECK_EQUAL(static_cast<int>(key.get_time_refresh_type()), static_cast<int>(refresh_type));
        BOOST_CHECK_EQUAL(key.get_parameter(), parameter);
        BOOST_CHECK_EQUAL(key.get_time_id(), time_id);
    }

    BOOST_AUTO_TEST_CASE(test_partial_constructor)
    {
        constexpr auto refresh_type = celeritas::time_refresh_type::weekly;
        constexpr auto parameter = 54321LL;

        const celeritas::player_time_refresh_key key{ refresh_type, parameter };

        BOOST_CHECK_EQUAL(static_cast<int>(key.get_time_refresh_type()), static_cast<int>(refresh_type));
        BOOST_CHECK_EQUAL(key.get_parameter(), parameter);
        BOOST_CHECK_EQUAL(key.get_time_id(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_player_time_refresh_constructor)
    {
        celeritas::player_time_refresh refresh_data{};
        refresh_data.set_time_refresh_type(celeritas::time_refresh_type::monthly);
        refresh_data.set_parameter(999);
        refresh_data.set_time_id(111);

        const celeritas::player_time_refresh_key key{ refresh_data };

        BOOST_CHECK_EQUAL(static_cast<int>(key.get_time_refresh_type()), static_cast<int>(refresh_data.get_time_refresh_type()));
        BOOST_CHECK_EQUAL(key.get_parameter(), refresh_data.get_parameter());
        BOOST_CHECK_EQUAL(key.get_time_id(), refresh_data.get_time_id());
    }

    BOOST_AUTO_TEST_CASE(test_comparison_operator)
    {
        const celeritas::player_time_refresh_key key1{ celeritas::time_refresh_type::daily, 100, 10 };
        const celeritas::player_time_refresh_key key2{ celeritas::time_refresh_type::daily, 100, 10 };
        const celeritas::player_time_refresh_key key3{ celeritas::time_refresh_type::daily, 100, 20 };
        const celeritas::player_time_refresh_key key4{ celeritas::time_refresh_type::daily, 200, 10 };
        const celeritas::player_time_refresh_key key5{ celeritas::time_refresh_type::weekly, 100, 10 };

        BOOST_CHECK(!(key1 < key2) && !(key2 < key1));

        BOOST_CHECK(key1 < key3);
        BOOST_CHECK(key1 < key4);
        BOOST_CHECK(key1 < key5);
        BOOST_CHECK(key4 < key5);
    }

    BOOST_AUTO_TEST_CASE(test_map_key)
    {
        std::map<celeritas::player_time_refresh_key, int> test_map{};

        const celeritas::player_time_refresh_key key1{ celeritas::time_refresh_type::daily, 1, 1 };
        const celeritas::player_time_refresh_key key2{ celeritas::time_refresh_type::weekly, 2, 2 };

        test_map[key1] = 100;
        test_map[key2] = 200;

        BOOST_CHECK_EQUAL(test_map.size(), 2);
        BOOST_CHECK_EQUAL(test_map.at(key1), 100);
        BOOST_CHECK_EQUAL(test_map.at(key2), 200);

        const celeritas::player_time_refresh_key key1_copy{ celeritas::time_refresh_type::daily, 1, 1 };
        BOOST_CHECK(test_map.count(key1_copy));

        test_map[key1_copy] = 101;
        BOOST_CHECK_EQUAL(test_map.at(key1), 101);
    }

BOOST_AUTO_TEST_SUITE_END()
