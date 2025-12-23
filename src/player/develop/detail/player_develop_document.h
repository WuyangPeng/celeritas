#pragma once

#include "config/config_fwd.h"
#include "database/database_data_type_traits.h"
#include "database/document/develop_data.h"
#include "player/develop/develop_data_key.h"

#include <boost/asio/awaitable.hpp>

#include <map>

namespace celeritas
{
    class player_develop_document
    {
    public:
        using class_type = player_develop_document;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using const_app_config_shared_ptr = std::shared_ptr<const app_config>;

        void set_develop(traits::param_type::document_array_type develop_document);

        [[nodiscard]] traits::document_array_type get_develop() const;

    private:
        using develop_data_container = std::map<develop_data_key, develop_data>;
        using const_item_config_shared_ptr = std::shared_ptr<const item_config>;

        void add_develop_data(const develop_data& develop_data);

        develop_data_container develop_data_;
    };
}
