#include "common/enum_cast.h"

#include <boost/test/unit_test.hpp>

namespace celeritas
{
    enum class test_enum_cast : uint8_t
    {
        zero,
        one,
        two,
        three,
        four,
    };
}

BOOST_AUTO_TEST_SUITE(enum_cast_tests)

    BOOST_AUTO_TEST_CASE(enum_cast)
    {
        BOOST_CHECK(celeritas::enum_cast_underlying(celeritas::test_enum_cast::one) == 1);
        BOOST_CHECK(celeritas::enum_cast_underlying<uint8_t>(celeritas::test_enum_cast::two) == 2);
        BOOST_CHECK(celeritas::underlying_cast_enum<celeritas::test_enum_cast>(3) == celeritas::test_enum_cast::three);

        celeritas::test_enum_cast e{};
        underlying_cast_enum_ptr(4, &e);
        BOOST_CHECK(e == celeritas::test_enum_cast::four);
        celeritas::underlying_cast_enum_ptr(5, static_cast<celeritas::test_enum_cast*>(nullptr));
    }

BOOST_AUTO_TEST_SUITE_END()

