#pragma once

#include "task_progress.h"
#include "config/luban/generated/schema.h"
#include "database/basic/database_data_type_traits.h"

#include <cstdint>
#include <vector>

namespace celeritas
{
    class task_component
    {
    public:
        using class_type = task_component;
        using document_type = traits::document_type;
        using task_progress_container = std::vector<task_progress>;

        task_component() noexcept = default;

        explicit task_component(config::task_component_type type);

        [[nodiscard]] config::task_component_type get_type() const;

        [[nodiscard]] const task_progress_container& get_task_progress() const;

        [[nodiscard]] task_progress_container& get_task_progress();

        void add_task_progress(const task_progress& progress);

        void remove_task_progress(int64_t progress_id);

        [[nodiscard]] document_type to_document_type() const;

        [[nodiscard]] static task_component from_document(const document_type& document);

        static constexpr std::string_view type_description = "type";
        static constexpr std::string_view task_progress_description = "task_progress";

    private:
        config::task_component_type type_ = config::task_component_type::max;
        task_progress_container task_progress_;
    };
}