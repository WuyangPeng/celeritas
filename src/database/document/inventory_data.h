#pragma once

#include "custom_data.h"

#include <cstdint>
#include <string>

namespace celeritas
{
    class inventory_data
    {
    public:
        using class_type = inventory_data;
        using document_view_type = bsoncxx::document::view;

        inventory_data() noexcept = default;

        inventory_data(int64_t item_id, int template_id, int64_t count, int position);

        [[nodiscard]] int64_t get_item_id() const;

        void set_item_id(int64_t item_id);

        [[nodiscard]] int get_template_id() const;

        void set_template_id(int template_id);

        [[nodiscard]] int64_t get_count() const;

        void set_count(int64_t count);

        [[nodiscard]] int get_position() const;

        void set_position(int position);

        [[nodiscard]] custom_data get_custom_data() const;

        void set_custom_data(const custom_data& custom_data);

        void set_custom_data(const document_view_type& document);

        void add_count(int64_t count);

        void reduce_count(int64_t count);

        [[nodiscard]] std::string to_json_string() const;

        [[nodiscard]] static inventory_data from_json_string(const std::string& json_string);

        static constexpr std::string_view item_id_description = "item_id";
        static constexpr std::string_view template_id_description = "template_id";
        static constexpr std::string_view count_description = "count";
        static constexpr std::string_view position_description = "position";
        static constexpr std::string_view custom_data_description = "custom_data";

    private:
        int64_t item_id_ = 0;
        int template_id_ = 0;
        int64_t count_ = 0;
        int position_ = 0;
        custom_data custom_data_;
    };
}