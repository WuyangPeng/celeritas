#include "database/document/custom_data.h"
#include "database/document/equipment_data.h"
#include "database/document/consumable_data.h"
#include "database/basic/basis_database.tpp"
#include "common/core/celeritas_error.h"

#include <boost/test/unit_test.hpp>

#include <string>
#include <vector>

BOOST_AUTO_TEST_SUITE(custom_data_suite)

    BOOST_AUTO_TEST_CASE(test_default_constructor)
    {
        celeritas::custom_data data;
        // 默认构造的 custom_data 应该是空的 (monostate)
        // 转换为文档应该为空
        BOOST_CHECK(data.to_document_type().empty());
    }

    BOOST_AUTO_TEST_CASE(test_equipment_round_trip)
    {
        // 构造 equipment_data 的文档
        celeritas::equipment_data::document_type equip_doc;
        equip_doc.emplace_back(celeritas::equipment_data::strength_description, 10);
        equip_doc.emplace_back(celeritas::equipment_data::durability_description, 100);

        // 构造 custom_data 的文档
        celeritas::custom_data::document_type custom_doc;
        custom_doc.emplace_back(celeritas::custom_data::type_description, std::string(celeritas::custom_data::equipment_description));
        custom_doc.emplace_back(celeritas::custom_data::data_description, equip_doc);

        // 从文档创建 custom_data
        auto data = celeritas::custom_data::from_document(custom_doc);

        // 再转回文档
        auto result_doc = data.to_document_type();

        // 验证结果
        bool found_type = false;
        bool found_data = false;

        for (const auto& element : result_doc)
        {
            if (element.get_field_name() == celeritas::custom_data::type_description)
            {
                BOOST_CHECK_EQUAL(element.get_value<celeritas::database_data_type::string_type>(), celeritas::custom_data::equipment_description);
                found_type = true;
            }
            else if (element.get_field_name() == celeritas::custom_data::data_description)
            {
                auto inner_doc = element.get_value<celeritas::database_data_type::document_type>();
                // 验证 inner_doc 内容
                for (const auto& inner_elem : inner_doc)
                {
                    if (inner_elem.get_field_name() == celeritas::equipment_data::strength_description)
                    {
                        BOOST_CHECK_EQUAL(inner_elem.get_value<celeritas::database_data_type::int32_type>(), 10);
                    }
                    else if (inner_elem.get_field_name() == celeritas::equipment_data::durability_description)
                    {
                        BOOST_CHECK_EQUAL(inner_elem.get_value<celeritas::database_data_type::int32_type>(), 100);
                    }
                }
                found_data = true;
            }
        }
        BOOST_CHECK(found_type);
        BOOST_CHECK(found_data);
    }

    BOOST_AUTO_TEST_CASE(test_consumable_round_trip)
    {
        // 构造 consumable_data 的文档
        celeritas::consumable_data::document_type cons_doc;
        cons_doc.emplace_back(celeritas::consumable_data::expire_time_description, 123456789LL);

        // 构造 custom_data 的文档
        celeritas::custom_data::document_type custom_doc;
        custom_doc.emplace_back(celeritas::custom_data::type_description, std::string(celeritas::custom_data::consumable_description));
        custom_doc.emplace_back(celeritas::custom_data::data_description, cons_doc);

        // 从文档创建 custom_data
        auto data = celeritas::custom_data::from_document(custom_doc);

        // 再转回文档
        auto result_doc = data.to_document_type();

        // 验证结果
        bool found_type = false;
        bool found_data = false;

        for (const auto& element : result_doc)
        {
            if (element.get_field_name() == celeritas::custom_data::type_description)
            {
                BOOST_CHECK_EQUAL(element.get_value<celeritas::database_data_type::string_type>(), celeritas::custom_data::consumable_description);
                found_type = true;
            }
            else if (element.get_field_name() == celeritas::custom_data::data_description)
            {
                auto inner_doc = element.get_value<celeritas::database_data_type::document_type>();
                // 验证 inner_doc 内容
                for (const auto& inner_elem : inner_doc)
                {
                    if (inner_elem.get_field_name() == celeritas::consumable_data::expire_time_description)
                    {
                        BOOST_CHECK_EQUAL(inner_elem.get_value<celeritas::database_data_type::int64_type>(), 123456789LL);
                    }
                }
                found_data = true;
            }
        }
        BOOST_CHECK(found_type);
        BOOST_CHECK(found_data);
    }

    BOOST_AUTO_TEST_CASE(test_empty_document)
    {
        celeritas::custom_data::document_type empty_doc;
        auto data = celeritas::custom_data::from_document(empty_doc);
        BOOST_CHECK(data.to_document_type().empty());
    }

    BOOST_AUTO_TEST_CASE(test_unknown_type)
    {
        celeritas::custom_data::document_type custom_doc;
        custom_doc.emplace_back(celeritas::custom_data::type_description, std::string("unknown"));

        // 应该抛出异常
        BOOST_CHECK_THROW(static_cast<void>(celeritas::custom_data::from_document(custom_doc)), celeritas::celeritas_error);
    }

BOOST_AUTO_TEST_SUITE_END()
