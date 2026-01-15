#include "database/redis/sorted_set_member_score.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(sorted_set_member_score_suite)

    BOOST_AUTO_TEST_CASE(test_sorted_set_member_score_constructor)
    {
        const std::string member{ "member1" };
        constexpr auto score = 10.5;
        const celeritas::sorted_set_member_score item{ member, score };

        BOOST_CHECK_EQUAL(item.get_member(), member);
        BOOST_CHECK_CLOSE(item.get_score(), score, 0.001);
    }

    BOOST_AUTO_TEST_CASE(test_sorted_set_member_score_empty_member)
    {
        const std::string member;
        constexpr auto score = 0.0;
        const celeritas::sorted_set_member_score item{ member, score };

        BOOST_CHECK_EQUAL(item.get_member(), member);
        BOOST_CHECK_CLOSE(item.get_score(), score, 0.001);
    }

    BOOST_AUTO_TEST_CASE(test_sorted_set_member_score_negative_score)
    {
        const std::string member{ "member2" };
        constexpr auto score = -5.5;
        const celeritas::sorted_set_member_score item{ member, score };

        BOOST_CHECK_EQUAL(item.get_member(), member);
        BOOST_CHECK_CLOSE(item.get_score(), score, 0.001);
    }

BOOST_AUTO_TEST_SUITE_END()
