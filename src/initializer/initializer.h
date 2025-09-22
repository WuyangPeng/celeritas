#pragma once

#include <memory>
#include <string>

namespace celeritas
{
    class initializer
    {
    public:
        using class_type = initializer;
        using initializer_unique_ptr = std::unique_ptr<initializer>;

        explicit initializer(std::string config_file_path) noexcept;

        virtual ~initializer() noexcept = default;

        initializer(const initializer& rhs) noexcept = default;

        initializer& operator=(const initializer& rhs) noexcept = default;

        initializer(initializer&& rhs) noexcept = default;

        initializer& operator=(initializer&& rhs) noexcept = default;

        void initialize();

        [[nodiscard]] static initializer_unique_ptr create_initializer(const std::string& server_type, const std::string& config_file_path);

    private:
        virtual void initialize_config() = 0;

        virtual void initialize_resource() = 0;

        virtual void initialize_application() = 0;

        std::string config_file_path_;
    };
}
