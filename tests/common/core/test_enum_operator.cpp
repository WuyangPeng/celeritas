#include "common/core/enum_operator.h"

#include <boost/test/unit_test.hpp>
#include <sstream>

namespace celeritas
{
    enum class test_enum_operator : uint32_t
    {
        zero,
        one,
        two,
        three,
        four,
        five,
    };

    enum class test_enum_operator_signed
    {
        zero,
        one,
    };
}

BOOST_AUTO_TEST_SUITE(enum_operator_tests)

    BOOST_AUTO_TEST_CASE(enum_operator)
    {
        BOOST_CHECK(celeritas::test_enum_operator::one + 1 == celeritas::test_enum_operator::two);
        BOOST_CHECK(1 + celeritas::test_enum_operator::one == celeritas::test_enum_operator::two);

        auto e1 = celeritas::test_enum_operator::one;
        e1 += 1;
        BOOST_CHECK(e1 == celeritas::test_enum_operator::two);

        BOOST_CHECK(celeritas::test_enum_operator::two - 1 == celeritas::test_enum_operator::one);

        auto e2 = celeritas::test_enum_operator::two;
        e2 -= 1;
        BOOST_CHECK(e2 == celeritas::test_enum_operator::one);

        BOOST_CHECK(celeritas::test_enum_operator::two * 2 == 4);
        BOOST_CHECK(2 * celeritas::test_enum_operator::two == 4);

        BOOST_CHECK(-celeritas::test_enum_operator_signed::one == celeritas::underlying_cast_enum<celeritas::test_enum_operator_signed>(-1));

        auto e3 = celeritas::test_enum_operator::one;
        ++e3;
        BOOST_CHECK(e3 == celeritas::test_enum_operator::two);
        const auto e4 = e3++;
        BOOST_CHECK(e4 == celeritas::test_enum_operator::two);
        BOOST_CHECK(e3 == celeritas::test_enum_operator::three);

        auto e5 = celeritas::test_enum_operator::two;
        --e5;
        BOOST_CHECK(e5 == celeritas::test_enum_operator::one);
        const auto e6 = e5--;
        BOOST_CHECK(e6 == celeritas::test_enum_operator::one);
        BOOST_CHECK(e5 == celeritas::test_enum_operator::zero);

        BOOST_CHECK((celeritas::test_enum_operator::three & celeritas::test_enum_operator::one) == celeritas::test_enum_operator::one);

        auto e7 = celeritas::test_enum_operator::three;
        e7 &= celeritas::test_enum_operator::one;
        BOOST_CHECK(e7 == celeritas::test_enum_operator::one);

        BOOST_CHECK((celeritas::test_enum_operator::one | celeritas::test_enum_operator::two) == celeritas::test_enum_operator::three);

        auto e8 = celeritas::test_enum_operator::one;
        e8 |= celeritas::test_enum_operator::two;
        BOOST_CHECK(e8 == celeritas::test_enum_operator::three);

        BOOST_CHECK((celeritas::test_enum_operator::three ^ celeritas::test_enum_operator::one) == celeritas::test_enum_operator::two);

        auto e9 = celeritas::test_enum_operator::three;
        e9 ^= celeritas::test_enum_operator::one;
        BOOST_CHECK(e9 == celeritas::test_enum_operator::two);

        std::stringstream ss1{};
        ss1 << celeritas::test_enum_operator::one;
        BOOST_CHECK_EQUAL(ss1.str(), "1");

        std::stringstream ss2{"2"};
        celeritas::test_enum_operator e10;
        ss2 >> e10;
        BOOST_CHECK_EQUAL(e10, celeritas::test_enum_operator::two);
    }

BOOST_AUTO_TEST_SUITE_END()

