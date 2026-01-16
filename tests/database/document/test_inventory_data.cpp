#include "database/document/inventory_data.h"
#include "database/document/custom_data.h"
#include "database/document/equipment_data.h"
#include "database/basic/basis_database.tpp"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(inventory_data_suite)

    BOOST_AUTO_TEST_CASE(test_default_constructor)
    {
        celeritas::inventory_data data;
        BOOST_CHECK_EQUAL(data.get_item_id(), 0);
        BOOST_CHECK_EQUAL(data.get_template_id(), 0);
        BOOST_CHECK_EQUAL(data.get_count(), 0);
        BOOST_CHECK_EQUAL(data.get_position(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_parameterized_constructor)
    {
        celeritas::inventory_data data(12345, 101, 5, 1);
        BOOST_CHECK_EQUAL(data.get_item_id(), 12345);
        BOOST_CHECK_EQUAL(data.get_template_id(), 101);
        BOOST_CHECK_EQUAL(data.get_count(), 5);
        BOOST_CHECK_EQUAL(data.get_position(), 1);
    }

    BOOST_AUTO_TEST_CASE(test_count_operations)
    {
        celeritas::inventory_data data;
        data.set_count(10);
        data.add_count(5);
        BOOST_CHECK_EQUAL(data.get_count(), 15);
        data.reduce_count(3);
        BOOST_CHECK_EQUAL(data.get_count(), 12);
    }

    BOOST_AUTO_TEST_CASE(test_custom_data_handling)
    {
        // 构造一个包含 equipment_data 的 custom_data
        celeritas::equipment_data::document_type equip_doc;
        equip_doc.emplace_back(celeritas::equipment_data::strength_description, 10);
        equip_doc.emplace_back(celeritas::equipment_data::durability_description, 100);

        celeritas::custom_data::document_type custom_doc;
        custom_doc.emplace_back(celeritas::custom_data::type_description, std::string(celeritas::custom_data::equipment_description));
        custom_doc.emplace_back(celeritas::custom_data::data_description, equip_doc);

        auto custom = celeritas::custom_data::from_document(custom_doc);

        celeritas::inventory_data inv_data;
        inv_data.set_custom_data(custom);

        // 验证 custom_data 是否被保存
        auto doc = inv_data.to_document_type();
        auto restored = celeritas::inventory_data::from_document(doc);

        auto restored_custom = restored.get_custom_data();
        auto restored_custom_doc = restored_custom.to_document_type();

        // 简单验证 custom_data 非空且包含正确类型
        bool found_type = false;
        for (const auto& elem : restored_custom_doc)
        {
            if (elem.get_field_name() == celeritas::custom_data::type_description)
            {
                BOOST_CHECK_EQUAL(elem.get_value<celeritas::database_data_type::string_type>(), celeritas::custom_data::equipment_description);
                found_type = true;
            }
        }
        BOOST_CHECK(found_type);
    }

BOOST_AUTO_TEST_SUITE_END()