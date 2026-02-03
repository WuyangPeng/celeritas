#include "language_data.h"
#include "boost/lexical_cast.hpp"
#include "common/core/enum_cast.h"
#include "database/basic/basis_database.tpp"

celeritas::language_data::language_data(const language_container_type& language)
    : language_{ language }
{
}

const celeritas::language_data::language_container_type& celeritas::language_data::get_language_map() const
{
    return language_;
}

void celeritas::language_data::set_language_map(const language_container_type& language)
{
    language_ = language;
}

std::string celeritas::language_data::get_language_text(const language_type language) const
{
    if (const auto iter = language_.find(language);
        iter != language_.end())
    {
        return iter->second;
    }
    return {};
}

void celeritas::language_data::set_language_text(const language_type language, const std::string& text)
{
    language_[language] = text;
}

void celeritas::language_data::clear()
{
    language_.clear();
}

bool celeritas::language_data::has_language(const language_type language) const
{
    return language_.contains(language);
}

int celeritas::language_data::size() const
{
    return boost::numeric_cast<int>(language_.size());
}

bool celeritas::language_data::empty() const
{
    return language_.empty();
}

celeritas::language_data::document_type celeritas::language_data::to_document_type() const
{
    document_type document{};

    for (const auto& [language, text] : language_)
    {
        const auto lang_name = language_description.data() + std::to_string(enum_cast_underlying(language));
        document.emplace_back(lang_name, text);
    }

    return document;
}

celeritas::language_data celeritas::language_data::from_document(const document_type& document)
{
    language_data language_data{};

    for (const auto& element : document)
    {
        if (const auto& field_name = element.get_field_name();
            field_name.substr(0, language_description.size()) == language_description)
        {
            const auto language_value = boost::lexical_cast<int>(field_name.substr(language_description.size()));
            const auto language = static_cast<language_type>(language_value);
            const auto& text = element.get_value<database_data_type::string_type>();
            language_data.set_language_text(language, text);
        }
    }

    return language_data;
}