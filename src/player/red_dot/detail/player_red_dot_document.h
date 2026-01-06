#pragma once

#include "database/basic/database_data_type_traits.h"
#include "database/document/red_dots.h"

#include <map>

namespace celeritas
{
    class player_red_dot_document
    {
    public:
        using class_type = player_red_dot_document;
        using red_dot_type = config::red_dot_type;

        void set_red_dots(traits::param_type::document_array_type red_dots_document);

        [[nodiscard]] traits::document_array_type get_red_dots() const;

        [[nodiscard]] bool insert_red_dot(red_dot_type red_dot, bool is_finish);

        [[nodiscard]] bool set_red_dot(red_dot_type red_dot, bool is_finish);

        [[nodiscard]] bool is_exist(red_dot_type red_dot) const;

        [[nodiscard]] bool is_state(red_dot_type red_dot) const;

    private:
        using red_dots_container = std::map<red_dot_type, red_dots>;

        red_dots_container red_dots_;
    };
}