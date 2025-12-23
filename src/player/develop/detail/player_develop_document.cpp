#include "player_develop_document.h"

#include <ranges>

void celeritas::player_develop_document::set_develop(traits::param_type::document_array_type develop_document)
{
    for (const auto& element : develop_document)
    {
        auto develop_data = develop_data::from_json_string(element);
        add_develop_data(develop_data);
    }
}

celeritas::traits::document_array_type celeritas::player_develop_document::get_develop() const
{
    traits::document_array_type documents{};
    for (auto& element : develop_data_ | std::views::values)
    {
        documents.emplace_back(element.to_json_string());
    }

    return documents;
}

void celeritas::player_develop_document::add_develop_data(const develop_data& develop_data)
{
    develop_data_.emplace(develop_data_key{ develop_data.get_system_id(), develop_data.get_instance_id() }, develop_data);
}