#pragma once

#include "generator.h"

#include <string>

namespace celeritas
{
    class save_mysql_file final : public generator
    {
    public:
        using class_type = save_mysql_file;
        using base_type = generator;

        explicit save_mysql_file(std::string output_directory, std::string content);

        void execute() override;

    private:
        std::string output_directory_;
        std::string content_;
    };
}