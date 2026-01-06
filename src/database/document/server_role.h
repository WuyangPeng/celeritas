#pragma once

#include "boost/asio/execution/allocator.hpp"
#include "database/basic/database_data_type_traits.h"

#include <cstdint>
#include <string>

namespace celeritas
{
    class server_role
    {
    public:
        using class_type = server_role;
        using document_type = traits::document_type;

        server_role();

        server_role(std::string game_server_id, std::string role_surname, std::string role_name);

        [[nodiscard]] std::string get_game_server_id() const;

        void set_game_server_id(const std::string& game_server_id);

        [[nodiscard]] std::string get_role_surname() const;

        void set_role_surname(const std::string& role_surname);

        [[nodiscard]] std::string get_role_name() const;

        void set_role_name(const std::string& role_name);

        [[nodiscard]] int64_t get_last_login_time() const;

        void set_last_login_time(int64_t last_login_time);

        [[nodiscard]] document_type to_document_type() const;

        [[nodiscard]] static server_role from_document(const document_type& document);

        static constexpr std::string_view game_server_id_description = "game_server_id";
        static constexpr std::string_view role_surname_description = "role_surname";
        static constexpr std::string_view role_name_description = "role_name";
        static constexpr std::string_view last_login_time_description = "last_login_time";

    private:
        std::string game_server_id_;
        std::string role_surname_;
        std::string role_name_;
        int64_t last_login_time_;
    };
}