#include "task_component.h"
#include "task_progress.h"
#include "common/core/enum_cast.h"
#include "config/luban/generated/schema.h"
#include "database/basic/basis_database.tpp"

celeritas::task_component::task_component(const config::task_component_type type)
    : type_{ type }
{
}

celeritas::config::task_component_type celeritas::task_component::get_type() const
{
    return type_;
}

const celeritas::task_component::task_progress_container& celeritas::task_component::get_task_progress() const
{
    return task_progress_;
}

celeritas::task_component::task_progress_container& celeritas::task_component::get_task_progress()
{
    return task_progress_;
}

void celeritas::task_component::add_task_progress(const task_progress& progress)
{
    task_progress_.emplace_back(progress);
}

void celeritas::task_component::remove_task_progress(const int64_t progress_id)
{
    std::erase_if(task_progress_,
                  [progress_id](const task_progress& progress) {
                      return progress.get_id() == progress_id;
                  });
}

celeritas::task_component::document_type celeritas::task_component::to_document_type() const
{
    document_type document{};

    document.emplace_back(type_description, enum_cast_underlying(type_));

    traits::document_array_type task_progress_document{};
    for (const auto& progress : task_progress_)
    {
        task_progress_document.emplace_back(progress.to_document_type());
    }
    document.emplace_back(task_progress_description, task_progress_document);

    return document;
}

celeritas::task_component celeritas::task_component::from_document(const document_type& document)
{
    task_component component{};

    for (const auto& element : document)
    {
        if (element.get_field_name() == type_description)
        {
            component.type_ = underlying_cast_enum<config::task_component_type>(element.get_value<database_data_type::int32_type>());
        }
        else if (element.get_field_name() == task_progress_description)
        {
            for (const auto& task_progress_array = element.get_value<database_data_type::document_array_type>();
                 const auto& progress_document : task_progress_array)
            {
                component.task_progress_.emplace_back(task_progress::from_document(progress_document));
            }
        }
    }

    return component;
}