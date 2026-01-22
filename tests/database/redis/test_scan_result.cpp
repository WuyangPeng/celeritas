#include "database/redis/scan_result.h"

#include <boost/test/unit_test.hpp>

#include <string>
#include <vector>

BOOST_AUTO_TEST_SUITE(scan_result_suite)

    BOOST_AUTO_TEST_CASE(test_scan_result_constructor_and_getters)
    {
        const std::string cursor{ "12345" };
        const celeritas::scan_result::array_type keys{ "key1", "key2", "key3" };

        const celeritas::scan_result result{ cursor, keys };
        BOOST_CHECK_EQUAL(result.get_cursor(), cursor);

        const auto retrieved_keys = result.get_keys();
        BOOST_CHECK_EQUAL(retrieved_keys.size(), keys.size());
        for (auto i = 0; i < keys.size(); ++i)
        {
            BOOST_CHECK_EQUAL(retrieved_keys.at(i), keys.at(i));
        }
    }

    BOOST_AUTO_TEST_CASE(test_scan_result_with_empty_cursor)
    {
        const std::string cursor{};
        const celeritas::scan_result::array_type keys{ "key1", "key2" };

        const celeritas::scan_result result{ cursor, keys };

        BOOST_CHECK_EQUAL(result.get_cursor(), cursor);
        BOOST_CHECK_EQUAL(result.get_keys().size(), 2);
        BOOST_CHECK_EQUAL(result.get_keys().at(0), "key1");
        BOOST_CHECK_EQUAL(result.get_keys().at(1), "key2");
    }

    BOOST_AUTO_TEST_CASE(test_scan_result_with_empty_keys)
    {
        const std::string cursor{ "67890" };
        const celeritas::scan_result::array_type keys{};

        const celeritas::scan_result result{ cursor, keys };

        BOOST_CHECK_EQUAL(result.get_cursor(), cursor);
        BOOST_CHECK(result.get_keys().empty());
    }

    BOOST_AUTO_TEST_CASE(test_scan_result_with_both_empty)
    {
        const std::string cursor{};
        const celeritas::scan_result::array_type keys{};

        const celeritas::scan_result result{ cursor, keys };

        BOOST_CHECK_EQUAL(result.get_cursor(), "");
        BOOST_CHECK(result.get_keys().empty());
    }

    BOOST_AUTO_TEST_CASE(test_scan_result_move_semantics)
    {
        std::string cursor{ "move_test_cursor" };
        celeritas::scan_result::array_type keys{ "move_key1", "move_key2", "move_key3" };

        const celeritas::scan_result result{ "temp_cursor",
                                             celeritas::scan_result::array_type{ "temp_key1", "temp_key2" } };

        BOOST_CHECK_EQUAL(result.get_cursor(), "temp_cursor");
        const auto retrieved_keys = result.get_keys();
        BOOST_CHECK_EQUAL(retrieved_keys.size(), 2);
        BOOST_CHECK_EQUAL(retrieved_keys.at(0), "temp_key1");
        BOOST_CHECK_EQUAL(retrieved_keys.at(1), "temp_key2");
    }

BOOST_AUTO_TEST_SUITE_END()
