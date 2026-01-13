#pragma once

#include "database/basic/database_data_type_traits.h"

namespace celeritas
{
    class consumable_data
    {
    public:
        using class_type = consumable_data;
        using document_type = traits::document_type;

        consumable_data();

        explicit consumable_data(int64_t expire_time);

        [[nodiscard]] int64_t get_expire_time() const;

        void set_expire_time(int64_t expire_time);

        [[nodiscard]] document_type to_document_type() const;

        [[nodiscard]] static consumable_data from_document(const document_type& document);

        static constexpr std::string_view expire_time_description = "expire_time";

    private:
        int64_t expire_time_;
    };
}
