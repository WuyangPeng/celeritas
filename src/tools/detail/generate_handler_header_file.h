#pragma once

#include "generator.h"

namespace celeritas
{
    class generate_handler_header_file final : public generator
    {
    public:
        using class_type = generate_handler_header_file;
        using base_type = generator;

        void execute() override;
    };
}