#define BOOST_TEST_MODULE "Celeritas Tests"

#include <boost/test/unit_test.hpp>

#include <any>

BOOST_AUTO_TEST_CASE(empty_test)
{
    BOOST_CHECK(true);
    std::any a{1LL};
    auto b = std::any_cast<int64_t>(a);
    BOOST_CHECK(b == 1LL);
}
