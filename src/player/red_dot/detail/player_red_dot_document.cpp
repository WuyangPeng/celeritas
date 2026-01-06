#include "player_red_dot_document.h"
#include "database/basic/basis_database.tpp"

#include <ranges>

void celeritas::player_red_dot_document::set_red_dots(traits::param_type::document_array_type red_dots_document)
{
    for (const auto& element : red_dots_document)
    {
        auto red_dots = red_dots::from_document(element);
        red_dots_.emplace(red_dots.get_node_id(), red_dots);
    }
}

celeritas::traits::document_array_type celeritas::player_red_dot_document::get_red_dots() const
{
    traits::document_array_type documents{};
    for (auto& element : red_dots_ | std::views::values)
    {
        documents.emplace_back(element.to_document_type());
    }

    return documents;
}

bool celeritas::player_red_dot_document::insert_red_dot(red_dot_type red_dot, const bool is_finish)
{
    if (const auto iter = red_dots_.find(red_dot);
        iter == red_dots_.cend())
    {
        red_dots_.emplace(red_dot, red_dots{ red_dot, is_finish });
        return true;
    }

    return false;
}

bool celeritas::player_red_dot_document::set_red_dot(red_dot_type red_dot, bool is_finish)
{
    const auto iter = red_dots_.find(red_dot);
    if (iter == red_dots_.cend())
    {
        red_dots_.emplace(red_dot, red_dots{ red_dot, is_finish });
        return true;
    }

    if (is_finish)
    {
        iter->second.set_state(is_finish);
        return true;
    }

    return false;
}

bool celeritas::player_red_dot_document::is_exist(const red_dot_type red_dot) const
{
    return red_dots_.contains(red_dot);
}

bool celeritas::player_red_dot_document::is_state(const red_dot_type red_dot) const
{
    if (const auto iter = red_dots_.find(red_dot);
        iter != red_dots_.cend())
    {
        return iter->second.is_state();
    }

    return false;
}