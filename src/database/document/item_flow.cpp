#include "item_flow.h"
#include "database/basic/basis_database.tpp"

celeritas::item_flow::item_flow()
    : action_{}, item_id_{}, template_id_{}, count_change_{}, before_count_{}, after_count_{}, source_id_{}
{
}

celeritas::item_flow::item_flow(std::string action, const int64_t item_id, const int32_t template_id, const int64_t count_change, const int64_t before_count, const int64_t after_count, std::string source_id)
    : action_{ std::move(action) },
      item_id_{ item_id },
      template_id_{ template_id },
      count_change_{ count_change },
      before_count_{ before_count },
      after_count_{ after_count },
      source_id_{ std::move(source_id) }
{
}

std::string celeritas::item_flow::get_action() const
{
    return action_;
}

void celeritas::item_flow::set_action(const std::string& action)
{
    action_ = action;
}

int64_t celeritas::item_flow::get_item_id() const
{
    return item_id_;
}

void celeritas::item_flow::set_item_id(const int64_t item_id)
{
    item_id_ = item_id;
}

int32_t celeritas::item_flow::get_template_id() const
{
    return template_id_;
}

void celeritas::item_flow::set_template_id(const int32_t template_id)
{
    template_id_ = template_id;
}

int64_t celeritas::item_flow::get_count_change() const
{
    return count_change_;
}

void celeritas::item_flow::set_count_change(const int64_t count_change)
{
    count_change_ = count_change;
}

int64_t celeritas::item_flow::get_before_count() const
{
    return before_count_;
}

void celeritas::item_flow::set_before_count(const int64_t before_count)
{
    before_count_ = before_count;
}

int64_t celeritas::item_flow::get_after_count() const
{
    return after_count_;
}

void celeritas::item_flow::set_after_count(const int64_t after_count)
{
    after_count_ = after_count;
}

std::string celeritas::item_flow::get_source_id() const
{
    return source_id_;
}

void celeritas::item_flow::set_source_id(const std::string& source_id)
{
    source_id_ = source_id;
}

celeritas::item_flow::document_type celeritas::item_flow::to_document_type() const
{
    document_type document{};

    document.emplace_back(action_description, action_);
    document.emplace_back(item_id_description, item_id_);
    document.emplace_back(template_id_description, template_id_);
    document.emplace_back(count_change_description, count_change_);
    document.emplace_back(before_count_description, before_count_);
    document.emplace_back(after_count_description, after_count_);
    document.emplace_back(source_id_description, source_id_);

    return document;
}

celeritas::item_flow celeritas::item_flow::from_document(const document_type& document)
{
    item_flow flow{};

    for (const auto& element : document)
    {
        if (element.get_field_name() == action_description)
        {
            flow.set_action(element.get_value<database_data_type::string_type>());
        }
        else if (element.get_field_name() == item_id_description)
        {
            flow.set_item_id(element.get_value<database_data_type::int64_type>());
        }
        else if (element.get_field_name() == template_id_description)
        {
            flow.set_template_id(element.get_value<database_data_type::int32_type>());
        }
        else if (element.get_field_name() == count_change_description)
        {
            flow.set_count_change(element.get_value<database_data_type::int64_type>());
        }
        else if (element.get_field_name() == before_count_description)
        {
            flow.set_before_count(element.get_value<database_data_type::int64_type>());
        }
        else if (element.get_field_name() == after_count_description)
        {
            flow.set_after_count(element.get_value<database_data_type::int64_type>());
        }
        else if (element.get_field_name() == source_id_description)
        {
            flow.set_source_id(element.get_value<database_data_type::string_type>());
        }
    }

    return flow;
}
