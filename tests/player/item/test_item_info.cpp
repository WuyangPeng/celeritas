#include "player/item/item_container.h"
#include "player/item/item_info.h"

#include <boost/test/unit_test.hpp>

using namespace celeritas;

BOOST_AUTO_TEST_SUITE(item_info_test_suite)

    BOOST_AUTO_TEST_CASE(test_item_info)
    {
        constexpr auto template_id = 1001;
        constexpr auto count = 50LL;
        const item_info info{ template_id, count };

        BOOST_CHECK_EQUAL(info.get_template_id(), template_id);
        BOOST_CHECK_EQUAL(info.get_count(), count);
    }

BOOST_AUTO_TEST_SUITE_END()
