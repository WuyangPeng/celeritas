#pragma once

#include "player_role_component.h"
#include "config/luban/generated/schema.h"
#include "player/component/player_service_base.h"
#include "proto/client/player/role.pb.h"

#include <boost/asio/awaitable.hpp>

namespace celeritas
{
    class change_name : public player_service_base
    {
    public:
        using class_type = change_name;
        using base_type = player_service_base;
        using request_type = proto::client::change_role_name_request;
        using change_name_shared_ptr = std::shared_ptr<class_type>;

        [[nodiscard]] static change_name_shared_ptr create(protobuf_handle_parameter_shared_ptr handle_parameter,
                                                           player_state_shared_ptr player_state,
                                                           request_type request);

        change_name(protobuf_handle_parameter_shared_ptr handle_parameter,
                    player_state_shared_ptr player_state,
                    request_type request);

        [[nodiscard]] void_awaitable_type response() override;

    private:
        using player_role_component_shared_ptr = std::shared_ptr<player_role_component>;
        using player_item_component_shared_ptr = std::shared_ptr<player_item_component>;
        using container_type = config::game::rename_cost_config_container;
        using priority_item_type = luban::SharedPtr<config::priority_item>;
        using optional_priority_item_type = std::optional<priority_item_type>;
        using priority_item_container_type = luban::Vector<priority_item_type>;
        using optional_priority_item_container_type = std::optional<priority_item_container_type>;

        [[nodiscard]] optional_priority_item_container_type get_rename_cost(const container_type& container) const;

        [[nodiscard]] optional_priority_item_type get_cost_item(const priority_item_container_type& priority_item);

        void cost_item(const priority_item_type& priority_item);

        request_type request_;
        player_role_component_shared_ptr player_role_component_;
        player_item_component_shared_ptr player_item_component_;
    };
}