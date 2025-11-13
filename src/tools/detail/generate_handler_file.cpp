#include "generate_handler_file.h"
#include "header_proto_generator.h"
#include "protobuf_compiler_importer.h"
#include "source_proto_generator.h"
#include "common/logger.h"
#include "tools/tools_fwd.h"

#include <boost/algorithm/string.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <google/protobuf/compiler/importer.h>

#include <fstream>

using namespace std::literals;

celeritas::generate_handler_file::generate_handler_file(std::string proto_file,
                                                        std::string proto_directory,
                                                        std::string output_directory,
                                                        const handler_template_file& handler_template_file)
    : proto_file_{ std::move(proto_file) },
      proto_directory_{ std::move(proto_directory) },
      output_directory_{ std::move(output_directory) },
      handler_template_file_{ handler_template_file }
{
}

void celeritas::generate_handler_file::generate_file()
{
    LOG_CHANNEL(celeritas::default_channel, trace) << "generate from file : " << proto_file_;

    protobuf_compiler_importer protobuf_compiler_importer{ proto_file_, proto_directory_ };

    if (const auto* file_descriptor = protobuf_compiler_importer.get_file_descriptor())
    {
        generate_file(file_descriptor);
    }
    else
    {
        LOG_CHANNEL(default_channel, error) << "Failed to import proto file. Check errors above.";
    }
}

void celeritas::generate_handler_file::generate_file(const file_descriptor_type file_descriptor) const
{
    for (auto index = 0; index < file_descriptor->message_type_count(); ++index)
    {
        if (const auto* message_descriptor = file_descriptor->message_type(index))
        {
            generate_file(file_descriptor, message_descriptor);
        }
        else
        {
            LOG_CHANNEL(default_channel, error) << "message descriptor is null.";
        }
    }
}

void celeritas::generate_handler_file::generate_file(const file_descriptor_type file_descriptor, const descriptor_type message_descriptor) const
{
    for (auto index = 0; index < message_descriptor->oneof_decl_count(); ++index)
    {
        if (const auto* one_of_descriptor = message_descriptor->oneof_decl(index))
        {
            generate_file(one_of_descriptor, file_descriptor, message_descriptor);
        }
        else
        {
            LOG_CHANNEL(default_channel, error) << "one of descriptor is null.";
        }
    }
}

void celeritas::generate_handler_file::generate_file(const one_of_descriptor_type one_of_descriptor, const file_descriptor_type file_descriptor, const descriptor_type message_descriptor) const
{
    if (const auto field = get_field(one_of_descriptor);
        !field.empty())
    {
        const auto file_name = file_descriptor->name();
        const auto message_full_name = message_descriptor->full_name();
        const auto message_name = message_descriptor->name();
        const auto one_of_name = one_of_descriptor->name();

        generate_file(file_name, message_full_name, message_name, one_of_name, field);
    }
}

celeritas::generate_handler_file::field_type celeritas::generate_handler_file::get_field(const one_of_descriptor_type one_of_descriptor) const
{
    field_type field{};
    for (auto index = 0; index < one_of_descriptor->field_count(); ++index)
    {
        if (const auto* field_desc = one_of_descriptor->field(index);
            field_desc != nullptr)
        {
            const auto field_name = field_desc->name();

            if (const auto message_type = field_desc->message_type();
                message_type != nullptr)
            {
                if (const auto field_message_type = message_type->name();
                    field_message_type.find(request_suffix) == std::string::npos &&
                    field_message_type.find(response_suffix) == std::string::npos)
                {
                    return field_type{};
                }

                field.emplace(field_name);
            }
        }
    }

    return field;
}

void celeritas::generate_handler_file::generate_file(const std::string_view file_name, const std::string_view message_full_name, const std::string_view message_name, const std::string_view one_of_name, const field_type& field) const
{
    generate_header_file(file_name, message_full_name, message_name);
    generate_source_file(message_full_name, message_name, one_of_name, field);
}

void celeritas::generate_handler_file::generate_header_file(const std::string_view file_name, const std::string_view message_full_name, const std::string_view message_name) const
{
    header_proto_generator header_proto_generator{ file_name, message_full_name, message_name, output_directory_, handler_template_file_ };
    header_proto_generator.execute();
}

void celeritas::generate_handler_file::generate_source_file(const std::string_view message_full_name, const std::string_view message_name, const std::string_view one_of_name, const field_type& field) const
{
    source_proto_generator source_proto_generator{ message_full_name, message_name, one_of_name, field, output_directory_, handler_template_file_ };
    source_proto_generator.execute();
}
