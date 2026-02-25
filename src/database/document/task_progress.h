#pragma once

#include "database_document_fwd.h"
#include "database/basic/database_data_type_traits.h"

#include <cstdint>
#include <string>

namespace celeritas
{
    class task_progress
    {
    public:
        using class_type = task_progress;
        using document_type = traits::document_type;

        task_progress() noexcept = default;

        task_progress(int64_t id, int cfg_id);

        task_progress(int64_t id, int cfg_id, int64_t progress);

        [[nodiscard]] int64_t get_id() const;

        [[nodiscard]] int get_cfg_id() const;

        [[nodiscard]] int get_progress() const;

        [[nodiscard]] task_status_type get_status() const;

        void set_progress(int64_t progress);

        void add_progress(int64_t progress);

        void set_status(task_status_type status);

        [[nodiscard]] document_type to_document_type() const;

        [[nodiscard]] static task_progress from_document(const document_type& document);

        static constexpr std::string_view id_description = "id";
        static constexpr std::string_view cfg_id_description = "cfg_id";
        static constexpr std::string_view progress_description = "progress";
        static constexpr std::string_view status_description = "status";

    private:
        int64_t id_;
        int cfg_id_;
        int64_t progress_;
        task_status_type status_;
    };
}