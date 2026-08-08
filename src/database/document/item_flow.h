#pragma once

#include "database/basic/database_data_type_traits.h"

#include <cstdint>
#include <string>

namespace celeritas
{
    class item_flow
    {
    public:
        using class_type = item_flow;
        using document_type = traits::document_type;

        item_flow();

        item_flow(std::string action, int64_t item_id, int32_t template_id, int64_t count_change, int64_t before_count, int64_t after_count, std::string source_id);

        [[nodiscard]] std::string get_action() const;

        void set_action(const std::string& action);

        [[nodiscard]] int64_t get_item_id() const;

        void set_item_id(int64_t item_id);

        [[nodiscard]] int32_t get_template_id() const;

        void set_template_id(int32_t template_id);

        [[nodiscard]] int64_t get_count_change() const;

        void set_count_change(int64_t count_change);

        [[nodiscard]] int64_t get_before_count() const;

        void set_before_count(int64_t before_count);

        [[nodiscard]] int64_t get_after_count() const;

        void set_after_count(int64_t after_count);

        [[nodiscard]] std::string get_source_id() const;

        void set_source_id(const std::string& source_id);

        [[nodiscard]] document_type to_document_type() const;

        [[nodiscard]] static item_flow from_document(const document_type& document);

        static constexpr std::string_view action_description = "action";
        static constexpr std::string_view item_id_description = "item_id";
        static constexpr std::string_view template_id_description = "template_id";
        static constexpr std::string_view count_change_description = "count_change";
        static constexpr std::string_view before_count_description = "before_count";
        static constexpr std::string_view after_count_description = "after_count";
        static constexpr std::string_view source_id_description = "source_id";

    private:
        std::string action_;
        int64_t item_id_;
        int32_t template_id_;
        int64_t count_change_;
        int64_t before_count_;
        int64_t after_count_;
        std::string source_id_;
    };
}
