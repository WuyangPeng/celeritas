#include "player/item/item_container.h"
#include "player/item/item_info.h"

#include <boost/test/unit_test.hpp>

using namespace celeritas;

BOOST_AUTO_TEST_SUITE(item_container_test_suite)

    BOOST_AUTO_TEST_CASE(test_item_container_add_and_iterate)
    {
        item_container container{};

        container.add_item_info(1001, 10);
        container.add_item_info(1002, 20);

        auto iter = container.begin();
        BOOST_CHECK(iter != container.end());
        BOOST_CHECK_EQUAL(iter->get_template_id(), 1001);
        BOOST_CHECK_EQUAL(iter->get_count(), 10);

        ++iter;
        BOOST_CHECK(iter != container.end());
        BOOST_CHECK_EQUAL(iter->get_template_id(), 1002);
        BOOST_CHECK_EQUAL(iter->get_count(), 20);

        ++iter;
        BOOST_CHECK(iter == container.end());
    }

BOOST_AUTO_TEST_SUITE_END()
