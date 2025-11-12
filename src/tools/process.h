#pragma once

#include <memory>
#include <string_view>

namespace celeritas
{
    class process
    {
    public:
        using class_type = process;
        using process_unique_ptr = std::unique_ptr<process>;

        process() noexcept = default;

        virtual ~process() noexcept = default;

        process(const process& rhs) noexcept = default;

        process& operator=(const process& rhs) noexcept = default;

        process(process&& rhs) noexcept = default;

        process& operator=(process&& rhs) noexcept = default;

        [[nodiscard]] static process_unique_ptr create_process(std::string_view process_name, std::string_view directory);

        virtual void execute() = 0;
    };
}