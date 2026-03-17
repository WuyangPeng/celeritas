#pragma once

#include "player_item_internal_fwd.h"
#include "config/config_fwd.h"

#include <memory>

namespace celeritas
{
    class player_resource
    {
    public:
        using class_type = player_resource;
        using const_app_config_shared_ptr = std::shared_ptr<const app_config>;

        explicit player_resource(player_item_document* player_item_document);

    private:
        player_item_document* player_item_document_;
    };
}
