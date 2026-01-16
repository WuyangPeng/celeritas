#include "database/basic/basis_database.tpp"
#include "database/document/consumable_data.h"

#include <boost/test/unit_test.hpp>

#include <string>
#include <vector>

namespace
{
    constexpr auto test_expire_time = 123456LL;

    void check_expire_time(const celeritas::consumable_data::document_type& document)
    {
        auto found_expire_time = false;
        for (const auto& element : document)
        {
            if (element.get_field_name() == celeritas::consumable_data::expire_time_description)
            {
                BOOST_CHECK_EQUAL(element.get_value<celeritas::database_data_type::int64_type>(), test_expire_time);
                found_expire_time = true;
                break;
            }
        }
        BOOST_CHECK_EQUAL(found_expire_time, true);
    }
}

BOOST_AUTO_TEST_SUITE(consumable_data_suite)

    BOOST_AUTO_TEST_CASE(test_default_constructor)
    {
        const celeritas::consumable_data data{};

        BOOST_CHECK_EQUAL(data.get_expire_time(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_parameterized_constructor)
    {
        constexpr auto test_expire_time = 123456789LL;
        const celeritas::consumable_data data{ test_expire_time };

        BOOST_CHECK_EQUAL(data.get_expire_time(), test_expire_time);
    }

    BOOST_AUTO_TEST_CASE(test_expire_time_accessors)
    {
        celeritas::consumable_data data{};
        constexpr auto test_value = 987654321LL;

        data.set_expire_time(test_value);

        BOOST_CHECK_EQUAL(data.get_expire_time(), test_value);
    }

    BOOST_AUTO_TEST_CASE(test_to_document_type)
    {
        const celeritas::consumable_data data{ test_expire_time };

        const auto document = data.to_document_type();

        check_expire_time(document);
    }

    BOOST_AUTO_TEST_CASE(test_from_document)
    {
        celeritas::consumable_data::document_type doc{};
        constexpr auto test_expire_time = 999888777LL;
        doc.emplace_back(celeritas::consumable_data::expire_time_description, test_expire_time);

        const auto data = celeritas::consumable_data::from_document(doc);

        BOOST_CHECK_EQUAL(data.get_expire_time(), test_expire_time);
    }

    BOOST_AUTO_TEST_CASE(test_from_document_empty)
    {
        const celeritas::consumable_data::document_type doc{};

        const auto data = celeritas::consumable_data::from_document(doc);

        BOOST_CHECK_EQUAL(data.get_expire_time(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_complete_data_flow)
    {
        constexpr auto original_expire_time = 111222333LL;
        const celeritas::consumable_data original_data{ original_expire_time };

        const auto document = original_data.to_document_type();

        const auto restored_data = celeritas::consumable_data::from_document(document);

        BOOST_CHECK_EQUAL(restored_data.get_expire_time(), original_expire_time);
    }

BOOST_AUTO_TEST_SUITE_END()