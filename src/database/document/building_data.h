#pragma once

#include "database/basic/database_data_type_traits.h"

namespace celeritas
{
    class building_data
    {
    public:
        using class_type = building_data;
        using document_type = traits::document_type;

        building_data();

        explicit building_data(int level);

        [[nodiscard]] int get_level() const;

        void set_level(int level);

        [[nodiscard]] document_type to_document_type() const;

        [[nodiscard]] static building_data from_document(const document_type& document);

        static constexpr std::string_view level_description = "level";

    private:
        int level_;
    };
}
