#pragma once

#include "common/core_utilities/command_line_config.h"

#include <memory>

namespace celeritas
{
    class process
    {
    public:
        using class_type = process;
        using process_unique_ptr = std::unique_ptr<process>;

        explicit process(command_line_config command_line_config);

        virtual ~process() noexcept = default;

        process(const process& rhs) noexcept = default;

        process& operator=(const process& rhs) noexcept = delete;

        process(process&& rhs) noexcept = default;

        process& operator=(process&& rhs) noexcept = delete;

        [[nodiscard]] static process_unique_ptr create_process(const command_line_config& command_line_config);

        virtual void execute() = 0;

    protected:
        static void check_directory_exists(const std::string& directory);

    private:
        command_line_config command_line_config_;
    };
}