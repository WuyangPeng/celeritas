#pragma once

#include "custom_data.h"
#include "database/basic/database_data_type_traits.h"

#include <cstdint>

namespace celeritas
{
    class inventory_data
    {
    public:
        using class_type = inventory_data;
        using document_type = traits::document_type;

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

        [[nodiscard]] bool is_locked() const;

        void set_is_locked(bool is_locked);

        [[nodiscard]] custom_data get_custom_data() const;

        void set_custom_data(const custom_data& custom_data);

        void add_count(int64_t count);

        void reduce_count(int64_t count);

        [[nodiscard]] document_type to_document_type() const;

        [[nodiscard]] static inventory_data from_document(const document_type& document);

        static constexpr std::string_view item_id_description = "item_id";
        static constexpr std::string_view template_id_description = "template_id";
        static constexpr std::string_view count_description = "count";
        static constexpr std::string_view position_description = "position";
        static constexpr std::string_view custom_data_description = "custom_data";
        static constexpr std::string_view is_locked_description = "is_locked";

    private:
        [[nodiscard]] static std::string_view get_custom_data(int template_id);

        int64_t item_id_ = 0;
        int template_id_ = 0;
        int64_t count_ = 0;
        int position_ = 0;
        bool is_locked_ = false;
        custom_data custom_data_;
    };
}