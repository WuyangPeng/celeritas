#pragma once

#include "generator.h"

#include <string>

namespace celeritas
{
    class save_mongo_file final : public generator
    {
    public:
        using class_type = save_mongo_file;
        using base_type = generator;

        explicit save_mongo_file(std::string file_name, std::string output_directory, std::string content);

        void execute() override;

    private:
        std::string file_name_;
        std::string output_directory_;
        std::string content_;
    };
}
