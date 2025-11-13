#pragma once

#include "handler_template_file.h"
#include "proto_generator.h"

namespace celeritas
{
    class header_proto_generator final : public proto_generator
    {
    public:
        using class_type = header_proto_generator;
        using base_type = proto_generator;

        header_proto_generator(std::string_view file_name,
                               std::string_view message_full_name,
                               std::string_view message_name,
                               std::string output_directory,
                               const handler_template_file& handler_template_file);

        void execute() override;

    private:
        std::string file_name_;
    };
}