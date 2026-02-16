#include "database/basic/basis_database.tpp"
#include "database/document/custom_data.h"
#include "database/document/equipment_data.h"
#include "database/document/inventory_data.h"
#include "config/game/game_config.h"
#include "config/game/game_tables.h"
#include "config/luban/generated/schema.h"

#include <boost/test/unit_test.hpp>

namespace
{
    void check_custom(const celeritas::custom_data::document_type& document)
    {
        auto found_type = false;
        for (const auto& element : document)
        {
            if (element.get_field_name() == celeritas::custom_data::type_description)
            {
                BOOST_CHECK_EQUAL(element.get_value<celeritas::database_data_type::string_type>(), celeritas::custom_data::equipment_description);
                found_type = true;
            }
        }
        BOOST_CHECK(found_type);
    }

    struct inventory_data_fixture
    {
        inventory_data_fixture()
        {
            init_game_config();
        }

        static void init_game_config()
        {
            const auto tables = std::make_shared<celeritas::config::tables>();
            const auto dataMap = const_cast<::luban::HashMap<::luban::int32, ::luban::SharedPtr<celeritas::config::game::item_config> >*>(&tables->item_config_container.getDataMap());

            // 添加测试用的物品配置
            auto item_config = std::make_shared<celeritas::config::game::item_config>();
            item_config->itemTemplateId = 101;
            item_config->itemType = celeritas::config::item_type::equipment;
            dataMap->emplace(item_config->itemTemplateId, item_config);

            const auto game_tables = std::make_shared<celeritas::game_tables>(tables);
            celeritas::game_config::get_instance().set_game_tables(game_tables);
        }
    };
}

BOOST_FIXTURE_TEST_SUITE(inventory_data_suite, inventory_data_fixture)

    BOOST_AUTO_TEST_CASE(test_default_constructor)
    {
        const celeritas::inventory_data data{};
        BOOST_CHECK_EQUAL(data.get_item_id(), 0);
        BOOST_CHECK_EQUAL(data.get_template_id(), 0);
        BOOST_CHECK_EQUAL(data.get_count(), 0);
        BOOST_CHECK_EQUAL(data.get_position(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_parameterized_constructor)
    {
        const celeritas::inventory_data data{ 12345, 101, 5, 1 };

        BOOST_CHECK_EQUAL(data.get_item_id(), 12345);
        BOOST_CHECK_EQUAL(data.get_template_id(), 101);
        BOOST_CHECK_EQUAL(data.get_count(), 5);
        BOOST_CHECK_EQUAL(data.get_position(), 1);
    }

    BOOST_AUTO_TEST_CASE(test_accessors)
    {
        celeritas::inventory_data data{};
        data.set_item_id(1);
        data.set_template_id(2);
        data.set_count(3);
        data.set_position(4);

        BOOST_CHECK_EQUAL(data.get_item_id(), 1);
        BOOST_CHECK_EQUAL(data.get_template_id(), 2);
        BOOST_CHECK_EQUAL(data.get_count(), 3);
        BOOST_CHECK_EQUAL(data.get_position(), 4);
    }

    BOOST_AUTO_TEST_CASE(test_count_operations)
    {
        celeritas::inventory_data data{};

        data.set_count(10);
        data.add_count(5);
        BOOST_CHECK_EQUAL(data.get_count(), 15);
        data.reduce_count(3);
        BOOST_CHECK_EQUAL(data.get_count(), 12);
    }

    BOOST_AUTO_TEST_CASE(test_custom_data_handling)
    {
        const celeritas::equipment_data::document_type equip_document{ celeritas::basis_database{ celeritas::equipment_data::strength_description, 10 },
                                                                       celeritas::basis_database{ celeritas::equipment_data::durability_description, 100 } };

        const celeritas::custom_data::document_type custom_document{ celeritas::basis_database{ celeritas::custom_data::type_description, std::string{ celeritas::custom_data::equipment_description } },
                                                                     celeritas::basis_database{ celeritas::custom_data::data_description, equip_document } };

        const auto custom = celeritas::custom_data::from_document(custom_document);

        celeritas::inventory_data inventory_data{};
        inventory_data.set_custom_data(custom);

        const auto document = inventory_data.to_document_type();
        const auto restored = celeritas::inventory_data::from_document(document);

        const auto restored_custom = restored.get_custom_data();
        const auto restored_custom_document = restored_custom.to_document_type();

        check_custom(restored_custom_document);
    }

    BOOST_AUTO_TEST_CASE(test_round_trip)
    {
        const celeritas::equipment_data::document_type equip_document{ celeritas::basis_database{ celeritas::equipment_data::strength_description, 10 },
                                                                       celeritas::basis_database{ celeritas::equipment_data::durability_description, 100 } };

        const celeritas::custom_data::document_type custom_document{ celeritas::basis_database{ celeritas::custom_data::type_description, std::string{ celeritas::custom_data::equipment_description } },
                                                                     celeritas::basis_database{ celeritas::custom_data::data_description, equip_document } };
        const auto custom = celeritas::custom_data::from_document(custom_document);

        celeritas::inventory_data original{ 12345, 101, 5, 1 };
        original.set_custom_data(custom);

        const auto document = original.to_document_type();
        const auto restored = celeritas::inventory_data::from_document(document);

        BOOST_CHECK_EQUAL(restored.get_item_id(), original.get_item_id());
        BOOST_CHECK_EQUAL(restored.get_template_id(), original.get_template_id());
        BOOST_CHECK_EQUAL(restored.get_count(), original.get_count());
        BOOST_CHECK_EQUAL(restored.get_position(), original.get_position());

        const auto restored_custom_document = restored.get_custom_data().to_document_type();
        check_custom(restored_custom_document);
    }

BOOST_AUTO_TEST_SUITE_END()