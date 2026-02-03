#pragma once

#include "language_type.h"
#include "database/basic/database_data_type_traits.h"

#include <map>
#include <string>

namespace celeritas
{
    class language_data
    {
    public:
        using class_type = language_data;
        using document_type = traits::document_type;
        using language_container_type = std::map<language_type, std::string>;

        language_data() = default;

        explicit language_data(const language_container_type& language);

        [[nodiscard]] const language_container_type& get_language_map() const;

        void set_language_map(const language_container_type& language);

        [[nodiscard]] std::string get_language_text(language_type language) const;

        void set_language_text(language_type language, const std::string& text);

        void clear();

        [[nodiscard]] bool has_language(language_type language) const;

        [[nodiscard]] int size() const;

        [[nodiscard]] bool empty() const;

        [[nodiscard]] document_type to_document_type() const;

        [[nodiscard]] static language_data from_document(const document_type& document);

        static constexpr std::string_view language_description = "language";

    private:
        language_container_type language_;
    };
}