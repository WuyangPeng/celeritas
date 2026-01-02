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

    BOOST_AUTO_TEST_CASE(test_addition)
    {
        BOOST_CHECK(celeritas::test_enum_operator::one + 1 == celeritas::test_enum_operator::two);
        BOOST_CHECK(1 + celeritas::test_enum_operator::one == celeritas::test_enum_operator::two);

        auto e = celeritas::test_enum_operator::one;
        e += 1;
        BOOST_CHECK(e == celeritas::test_enum_operator::two);
    }

    BOOST_AUTO_TEST_CASE(test_subtraction)
    {
        BOOST_CHECK(celeritas::test_enum_operator::two - 1 == celeritas::test_enum_operator::one);

        auto e = celeritas::test_enum_operator::two;
        e -= 1;
        BOOST_CHECK(e == celeritas::test_enum_operator::one);
    }

    BOOST_AUTO_TEST_CASE(test_multiplication)
    {
        BOOST_CHECK(celeritas::test_enum_operator::two * 2 == 4);
        BOOST_CHECK(2 * celeritas::test_enum_operator::two == 4);
    }

    BOOST_AUTO_TEST_CASE(test_unary_negation)
    {
        BOOST_CHECK(-celeritas::test_enum_operator_signed::one == celeritas::underlying_cast_enum<celeritas::test_enum_operator_signed>(-1));
    }

    BOOST_AUTO_TEST_CASE(test_increment)
    {
        auto e1 = celeritas::test_enum_operator::one;
        ++e1;
        BOOST_CHECK(e1 == celeritas::test_enum_operator::two);
        const auto e2 = e1++;
        BOOST_CHECK(e2 == celeritas::test_enum_operator::two);
        BOOST_CHECK(e1 == celeritas::test_enum_operator::three);
    }

    BOOST_AUTO_TEST_CASE(test_decrement)
    {
        auto e1 = celeritas::test_enum_operator::two;
        --e1;
        BOOST_CHECK(e1 == celeritas::test_enum_operator::one);
        const auto e2 = e1--;
        BOOST_CHECK(e2 == celeritas::test_enum_operator::one);
        BOOST_CHECK(e1 == celeritas::test_enum_operator::zero);
    }

    BOOST_AUTO_TEST_CASE(test_bitwise_and)
    {
        BOOST_CHECK((celeritas::test_enum_operator::three & celeritas::test_enum_operator::one) == celeritas::test_enum_operator::one);

        auto e = celeritas::test_enum_operator::three;
        e &= celeritas::test_enum_operator::one;
        BOOST_CHECK(e == celeritas::test_enum_operator::one);
    }

    BOOST_AUTO_TEST_CASE(test_bitwise_or)
    {
        BOOST_CHECK((celeritas::test_enum_operator::one | celeritas::test_enum_operator::two) == celeritas::test_enum_operator::three);

        auto e = celeritas::test_enum_operator::one;
        e |= celeritas::test_enum_operator::two;
        BOOST_CHECK(e == celeritas::test_enum_operator::three);
    }

    BOOST_AUTO_TEST_CASE(test_bitwise_xor)
    {
        BOOST_CHECK((celeritas::test_enum_operator::three ^ celeritas::test_enum_operator::one) == celeritas::test_enum_operator::two);

        auto e = celeritas::test_enum_operator::three;
        e ^= celeritas::test_enum_operator::one;
        BOOST_CHECK(e == celeritas::test_enum_operator::two);
    }

    BOOST_AUTO_TEST_CASE(test_stream_output)
    {
        std::stringstream ss{};
        ss << celeritas::test_enum_operator::one;
        BOOST_CHECK_EQUAL(ss.str(), "1");
    }

    BOOST_AUTO_TEST_CASE(test_stream_input)
    {
        std::stringstream ss{ "2" };
        celeritas::test_enum_operator e;
        ss >> e;
        BOOST_CHECK_EQUAL(e, celeritas::test_enum_operator::two);
    }

BOOST_AUTO_TEST_SUITE_END()
