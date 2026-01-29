#include "player_item_selected_document.h"
#include "common/core/snowflake_generator.h"
#include "config/aggregate/app_config.h"
#include "config/aggregate/server_config.h"
#include "database/basic/basis_database.tpp"

#include <ranges>

void celeritas::player_item_selected_document::set_item_selected(traits::param_type::document_array_type item_selected_document)
{
    for (const auto& element : item_selected_document)
    {
        auto item_selected_data = item_selected_data::from_document(element);
        add_item_selected_data(item_selected_data);
    }
}

celeritas::traits::document_array_type celeritas::player_item_selected_document::get_item_selected() const
{
    traits::document_array_type documents{};
    for (auto& element : item_selected_ | std::views::values)
    {
        documents.emplace_back(element.to_document_type());
    }

    return documents;
}

bool celeritas::player_item_selected_document::change_item_selected(const const_app_config_shared_ptr& app_config,
                                                                    const config::item_type item_type,
                                                                    const config::item_selected_child_type child_type,
                                                                    const int64_t operation_id,
                                                                    const int parameter,
                                                                    const int64_t selected_id)
{
    const item_selected_key key{ item_type, child_type, operation_id, parameter };
    if (const auto iter = item_selected_.find(key);
        iter != item_selected_.cend())
    {
        if (selected_id == iter->second.get_selected_id())
        {
            return false;
        }

        iter->second.set_selected_id(selected_id);
    }
    else
    {
        if (selected_id == 0)
        {
            return false;
        }

        const auto server_config = app_config->get_server_config();

        item_selected_data data{};

        data.set_id(snowflake_generator::get_instance().generate(server_config->get_datacenter_id(), server_config->get_worker_id()));
        data.set_item_type(item_type);
        data.set_child_type(child_type);
        data.set_operation_id(operation_id);
        data.set_parameter(parameter);
        data.set_selected_id(selected_id);

        item_selected_.emplace(key, data);
    }

    return true;
}

void celeritas::player_item_selected_document::add_item_selected_data(const item_selected_data& item_selected)
{
    item_selected_.emplace(item_selected_key{ item_selected.get_item_type(), item_selected.get_child_type(), item_selected.get_operation_id(), item_selected.get_parameter() }, item_selected);
}