#include "task_data.h"
#include "task_component.h"
#include "database/basic/basis_database.tpp"

const celeritas::task_data::task_component_container& celeritas::task_data::get_task_components() const
{
    return task_components_;
}

celeritas::task_data::task_component_container& celeritas::task_data::get_task_components()
{
    return task_components_;
}

void celeritas::task_data::add_task_component(const task_component& component)
{
    task_components_.emplace_back(component);
}

celeritas::task_data::document_type celeritas::task_data::to_document_type() const
{
    document_type document{};

    traits::document_array_type task_components_document{};
    for (const auto& component : task_components_)
    {
        task_components_document.emplace_back(component.to_document_type());
    }
    document.emplace_back(task_components_description, task_components_document);

    return document;
}

celeritas::task_data celeritas::task_data::from_document(const document_type& document)
{
    task_data data{};

    for (const auto& element : document)
    {
        if (element.get_field_name() == task_components_description)
        {
            for (const auto& task_components_array = element.get_value<database_data_type::document_array_type>();
                 const auto& component_document : task_components_array)
            {
                data.task_components_.emplace_back(task_component::from_document(component_document));
            }
        }
    }

    return data;
}