#pragma once

#include <string>

namespace celeritas
{
    class generator
    {
    public:
        using class_type = generator;

        generator() noexcept = default;

        virtual ~generator() noexcept = default;

        generator(const generator& rhs) noexcept = default;

        generator& operator=(const generator& rhs) noexcept = delete;

        generator(generator&& rhs) noexcept = default;

        generator& operator=(generator&& rhs) noexcept = delete;

        virtual void execute() = 0;

        [[nodiscard]] static bool is_content_same(const std::string& file_name, const std::string& content);

        static void save_handler(const std::string& file_name, const std::string& content);
    };
}