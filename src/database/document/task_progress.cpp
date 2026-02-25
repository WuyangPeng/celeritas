#include "task_progress.h"
#include "task_status_type.h"
#include "common/core/enum_cast.h"
#include "database/basic/basis_database.tpp"

celeritas::task_progress::task_progress(const int64_t id, const int cfg_id)
    : class_type{ id, cfg_id, 0 }
{
}

celeritas::task_progress::task_progress(const int64_t id, const int cfg_id, const int64_t progress)
    : id_{ id }, cfg_id_{ cfg_id }, progress_{ progress }, status_{ task_status_type::in_progress }
{
}

int64_t celeritas::task_progress::get_id() const
{
    return id_;
}

int celeritas::task_progress::get_cfg_id() const
{
    return cfg_id_;
}

int celeritas::task_progress::get_progress() const
{
    return progress_;
}

celeritas::task_status_type celeritas::task_progress::get_status() const
{
    return status_;
}

void celeritas::task_progress::set_progress(const int64_t progress)
{
    progress_ = progress;
}

void celeritas::task_progress::add_progress(const int64_t progress)
{
    progress_ += progress;
}

void celeritas::task_progress::set_status(const task_status_type status)
{
    status_ = status;
}

celeritas::task_progress::document_type celeritas::task_progress::to_document_type() const
{
    document_type document{};

    document.emplace_back(id_description, id_);
    document.emplace_back(cfg_id_description, cfg_id_);
    document.emplace_back(progress_description, progress_);
    document.emplace_back(status_description, static_cast<int>(status_));

    return document;
}

celeritas::task_progress celeritas::task_progress::from_document(const document_type& document)
{
    task_progress task{};

    for (const auto& element : document)
    {
        if (element.get_field_name() == id_description)
        {
            task.id_ = element.get_value<database_data_type::int64_type>();
        }
        else if (element.get_field_name() == cfg_id_description)
        {
            task.cfg_id_ = element.get_value<database_data_type::int32_type>();
        }
        else if (element.get_field_name() == progress_description)
        {
            task.progress_ = element.get_value<database_data_type::int32_type>();
        }
        else if (element.get_field_name() == status_description)
        {
            task.status_ = underlying_cast_enum<task_status_type>(element.get_value<database_data_type::int32_type>());
        }
    }

    return task;
}