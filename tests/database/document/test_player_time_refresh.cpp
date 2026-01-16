#include "database/document/player_time_refresh.h"
#include "player/time/time_refresh_type.h"
#include "player/component/player_component_type.h"
#include "database/basic/basis_database.tpp"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(player_time_refresh_suite)

    BOOST_AUTO_TEST_CASE(test_default_constructor)
    {
        celeritas::player_time_refresh data;
        BOOST_CHECK_EQUAL(data.get_parameter(), 0);
        BOOST_CHECK_EQUAL(data.get_time_id(), 0);
        BOOST_CHECK_EQUAL(data.get_last_refresh_time(), 0);
        BOOST_CHECK(data.get_component().empty());
    }

    BOOST_AUTO_TEST_CASE(test_component_operations)
    {
        celeritas::player_time_refresh data;
        // 使用实际的枚举值，假设枚举有值
        // 为了安全起见，我们可以强制转换整数，因为我们只是测试容器操作
        auto comp1 = static_cast<celeritas::player_component_type>(1);
        auto comp2 = static_cast<celeritas::player_component_type>(2);

        data.add_component(comp1);
        BOOST_CHECK_EQUAL(data.get_component().size(), 1);

        data.add_component(comp2);
        BOOST_CHECK_EQUAL(data.get_component().size(), 2);

        data.remove_component(comp1);
        BOOST_CHECK_EQUAL(data.get_component().size(), 1);
        BOOST_CHECK(data.get_component()[0] == comp2);
    }

    BOOST_AUTO_TEST_CASE(test_round_trip)
    {
        celeritas::player_time_refresh original;
        original.set_parameter(100);
        original.set_time_id(1);
        original.set_last_refresh_time(123456789);
        original.add_component(static_cast<celeritas::player_component_type>(10));
        original.set_time_refresh_type(celeritas::time_refresh_type::daily);

        auto doc = original.to_document_type();
        auto restored = celeritas::player_time_refresh::from_document(doc);

        BOOST_CHECK_EQUAL(restored.get_parameter(), 100);
        BOOST_CHECK_EQUAL(restored.get_time_id(), 1);
        BOOST_CHECK_EQUAL(restored.get_last_refresh_time(), 123456789);
        BOOST_CHECK_EQUAL(restored.get_component().size(), 1);
        BOOST_CHECK(restored.get_time_refresh_type() == celeritas::time_refresh_type::daily);
    }

BOOST_AUTO_TEST_SUITE_END()