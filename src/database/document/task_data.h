#pragma once

#include "database_document_fwd.h"
#include "database/basic/database_data_type_traits.h"

#include <cstdint>
#include <vector>

namespace celeritas
{
    class task_data
    {
    public:
        using class_type = task_data;
        using document_type = traits::document_type;
        using task_component_container = std::vector<task_component>;

        task_data() noexcept = default;

        [[nodiscard]] const task_component_container& get_task_components() const;

        [[nodiscard]] task_component_container& get_task_components();

        void add_task_component(const task_component& component);

        [[nodiscard]] document_type to_document_type() const;

        [[nodiscard]] static task_data from_document(const document_type& document);

        static constexpr std::string_view task_components_description = "task_components";

    private:
        task_component_container task_components_;
    };
}