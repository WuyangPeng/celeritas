#include "common/core/celeritas_error.h"
#include "database/basic/basis_database.tpp"
#include "database/document/consumable_data.h"
#include "database/document/custom_data.h"
#include "database/document/equipment_data.h"

#include <boost/test/unit_test.hpp>

#include <string>
#include <vector>

namespace
{
    void check_equipment(const celeritas::custom_data::document_type& document)
    {
        auto found_type = false;
        auto found_data = false;

        for (const auto& element : document)
        {
            if (element.get_field_name() == celeritas::custom_data::type_description)
            {
                BOOST_CHECK_EQUAL(element.get_value<celeritas::database_data_type::string_type>(), celeritas::custom_data::equipment_description);
                found_type = true;
            }
            else if (element.get_field_name() == celeritas::custom_data::data_description)
            {
                for (const auto& inner_document = element.get_value<celeritas::database_data_type::document_type>();
                     const auto& inner_element : inner_document)
                {
                    if (inner_element.get_field_name() == celeritas::equipment_data::strength_description)
                    {
                        BOOST_CHECK_EQUAL(inner_element.get_value<celeritas::database_data_type::int32_type>(), 10);
                    }
                    else if (inner_element.get_field_name() == celeritas::equipment_data::durability_description)
                    {
                        BOOST_CHECK_EQUAL(inner_element.get_value<celeritas::database_data_type::int32_type>(), 100);
                    }
                }
                found_data = true;
            }
        }
        BOOST_CHECK(found_type);
        BOOST_CHECK(found_data);
    }

    void check_consumable(const celeritas::custom_data::document_type& document)
    {
        auto found_type = false;
        auto found_data = false;

        for (const auto& element : document)
        {
            if (element.get_field_name() == celeritas::custom_data::type_description)
            {
                BOOST_CHECK_EQUAL(element.get_value<celeritas::database_data_type::string_type>(), celeritas::custom_data::consumable_description);
                found_type = true;
            }
            else if (element.get_field_name() == celeritas::custom_data::data_description)
            {
                for (const auto& inner_document = element.get_value<celeritas::database_data_type::document_type>();
                     const auto& inner_element : inner_document)
                {
                    if (inner_element.get_field_name() == celeritas::consumable_data::expire_time_description)
                    {
                        BOOST_CHECK_EQUAL(inner_element.get_value<celeritas::database_data_type::int64_type>(), 123456789LL);
                    }
                }
                found_data = true;
            }
        }
        BOOST_CHECK(found_type);
        BOOST_CHECK(found_data);
    }
}

BOOST_AUTO_TEST_SUITE(custom_data_suite)

    BOOST_AUTO_TEST_CASE(test_default_constructor)
    {
        const celeritas::custom_data data{};

        BOOST_CHECK(data.to_document_type().empty());
    }

    BOOST_AUTO_TEST_CASE(test_equipment_round_trip)
    {
        const celeritas::equipment_data::document_type equip_document{ celeritas::basis_database{ celeritas::equipment_data::strength_description, 10 },
                                                                       celeritas::basis_database{ celeritas::equipment_data::durability_description, 100 } };

        const celeritas::custom_data::document_type custom_document{ celeritas::basis_database{ celeritas::custom_data::type_description, std::string{ celeritas::custom_data::equipment_description } },
                                                                     celeritas::basis_database{ celeritas::custom_data::data_description, equip_document } };

        const auto data = celeritas::custom_data::from_document(custom_document);

        const auto result_document = data.to_document_type();

        check_equipment(result_document);
    }

    BOOST_AUTO_TEST_CASE(test_consumable_round_trip)
    {
        const celeritas::consumable_data::document_type consumable_document{ celeritas::basis_database{ celeritas::consumable_data::expire_time_description, 123456789LL } };

        const celeritas::custom_data::document_type custom_document{ celeritas::basis_database{ celeritas::custom_data::type_description, std::string{ celeritas::custom_data::consumable_description } },
                                                                     celeritas::basis_database{ celeritas::custom_data::data_description, consumable_document } };

        const auto custom_data = celeritas::custom_data::from_document(custom_document);
        const auto result_document = custom_data.to_document_type();

        check_consumable(result_document);
    }

    BOOST_AUTO_TEST_CASE(test_empty_document)
    {
        const celeritas::custom_data::document_type empty_document{};
        const auto data = celeritas::custom_data::from_document(empty_document);
        BOOST_CHECK(data.to_document_type().empty());
    }

    BOOST_AUTO_TEST_CASE(test_unknown_type)
    {
        celeritas::custom_data::document_type custom_document{ celeritas::basis_database{ celeritas::custom_data::type_description, std::string{ "unknown" } } };

        BOOST_CHECK_THROW(std::ignore = celeritas::custom_data::from_document(custom_document), celeritas::celeritas_error);
    }

BOOST_AUTO_TEST_SUITE_END()
