#include "player_item_selected_document.h"
#include "common/core/enum_cast.h"
#include "common/core/snowflake_generator.h"
#include "common/logging/logger.h"
#include "config/aggregate/app_config.h"
#include "config/aggregate/server_config.h"
#include "database/basic/basis_database.tpp"
#include "initializer/initializer_constant.h"
#include "proto/celeritas.pb.h"

#include <ranges>

celeritas::player_item_selected_document::player_item_selected_document(player_state* player_state)
    : player_state_{ player_state }
{
}

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

celeritas::player_item_selected_document::optional_item_selected_data celeritas::player_item_selected_document::change_item_selected(const const_app_config_shared_ptr& app_config,
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
            return std::nullopt;
        }

        iter->second.set_selected_id(selected_id);

        return iter->second;
    }

    if (selected_id == 0)
    {
        return std::nullopt;
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

    return data;
}

void celeritas::player_item_selected_document::on_dependencies_ready()
{
    send_item_message(true, 0, item_selected_);
}

void celeritas::player_item_selected_document::send_item_message(const bool is_login, const int rpc, const item_selected_data_container& item_selected)
{
    const header header{ rpc, player_state_->get_user_id() };

    proto::celeritas response{};
    auto* item_selected_response = response.mutable_celeritas_response()->mutable_client()->mutable_player()->mutable_item()->mutable_item_selected();
    item_selected_response->set_is_login(is_login);
    for (const auto& element : item_selected | std::views::values)
    {
        auto* data = item_selected_response->add_item_selected();
        data->set_id(element.get_id());
        data->set_item_type(enum_cast_underlying(element.get_item_type()));
        data->set_child_type(enum_cast_underlying(element.get_child_type()));
        data->set_operation_id(element.get_operation_id());
        data->set_parameter(element.get_parameter());
        data->set_selected_id(element.get_selected_id());
    }

    if (!player_state_->write(gateway_type.data(), player_state_->get_instance_id(), header, response))
    {
        LOG_CHANNEL(player_channel, error) << "send message error.";
    }
}

void celeritas::player_item_selected_document::add_item_selected_data(const item_selected_data& item_selected)
{
    item_selected_.emplace(item_selected_key{ item_selected.get_item_type(), item_selected.get_child_type(), item_selected.get_operation_id(), item_selected.get_parameter() }, item_selected);
}