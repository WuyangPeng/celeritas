#pragma once

#include <bsoncxx/builder/basic/document.hpp>

namespace celeritas
{
    class consumable_data
    {
    public:
        using class_type = consumable_data;
        using document_type = bsoncxx::builder::basic::document;
        using document_view_type = bsoncxx::document::view;

        consumable_data();

        explicit consumable_data(const document_view_type& document_view);

        explicit consumable_data(int64_t expire_time);

        [[nodiscard]] int64_t get_expire_time() const;

        void set_expire_time(int64_t expire_time);

        [[nodiscard]] document_type to_document_type() const;

        void set_document(const document_view_type& document_view);

        static constexpr std::string_view expire_time_description = "expire_time";

    private:
        int64_t expire_time_;
    };
}
