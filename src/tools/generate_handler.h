#pragma once

#include "process.h"

namespace celeritas
{
    class generate_handler final : public process
    {
    public:
        using class_type = generate_handler;
        using base_type = process;

        explicit generate_handler(std::string_view directory);

        void execute() override;

    private:
        std::string directory_;
    };
}