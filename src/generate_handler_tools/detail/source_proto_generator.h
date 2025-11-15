#pragma once

#include "proto_generator.h"

#include <set>

namespace celeritas
{
    class source_proto_generator final : public proto_generator
    {
    public:
        using class_type = source_proto_generator;
        using base_type = proto_generator;
        using field_type = std::set<std::string>;

        source_proto_generator(std::string_view message_full_name,
                               std::string_view message_name,
                               std::string_view one_of_name,
                               const field_type& field,
                               std::string output_directory,
                               const handler_template_file& handler_template_file);

        void execute() override;

    private:
        [[nodiscard]] std::string get_payload_name() const;

        [[nodiscard]] std::string get_add_handler_function(const std::string& payload_name, const std::string& proto_full_name, const std::string& message_name) const;

        [[nodiscard]] static std::string get_entry_enum_name(const std::string& field);

        std::string one_of_name_;
        field_type field_;
    };
}