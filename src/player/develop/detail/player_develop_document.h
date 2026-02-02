#pragma once

#include "config/config_fwd.h"
#include "database/basic/database_data_type_traits.h"
#include "database/document/develop_data.h"
#include "message/message_fwd.h"
#include "player/component/player_state.h"
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
        using optional_develop_data = std::optional<develop_data>;

        explicit player_develop_document(player_state* player_state);

        void set_develop(traits::param_type::document_array_type develop_document);

        [[nodiscard]] traits::document_array_type get_develop() const;

        [[nodiscard]] game_error_type develop_level(const develop_data_key& key);

        [[nodiscard]] game_error_type develop_reset(const develop_data_key& key);

        void send_initial_sync();

        [[nodiscard]] optional_develop_data get_develop_data(const develop_data_key& key) const;

    private:
        using develop_data_container = std::map<develop_data_key, develop_data>;

        void add_develop_data(const develop_data& develop_data);

        develop_data_container develop_data_;
        player_state* player_state_;
    };
}
