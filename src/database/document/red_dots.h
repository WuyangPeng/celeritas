#pragma once

#include "config/luban/generated/schema.h"
#include "database/basic/database_data_type_traits.h"

#include <cstdint>

namespace celeritas
{
    class red_dots
    {
    public:
        using class_type = red_dots;
        using red_dot_type = config::red_dot_type;
        using document_type = traits::document_type;

        red_dots();

        red_dots(red_dot_type node_id, bool state);

        [[nodiscard]] red_dot_type get_node_id() const;

        void set_node_id(red_dot_type nodeId);

        [[nodiscard]] bool is_state() const;

        void set_state(bool state);

        [[nodiscard]] int64_t get_last_value() const;

        void set_last_value(int64_t lastValue);

        [[nodiscard]] int64_t get_update_time() const;

        void set_update_time(int64_t updateTime);

        [[nodiscard]] document_type to_document_type() const;

        [[nodiscard]] static red_dots from_document(const document_type& document);

        static constexpr std::string_view node_id_description = "node_id";
        static constexpr std::string_view state_description = "state";
        static constexpr std::string_view last_value_description = "last_value";
        static constexpr std::string_view update_time_description = "update_time";

    private:
        red_dot_type node_id_;
        bool state_;
        int64_t last_value_;
        int64_t update_time_;
    };
}