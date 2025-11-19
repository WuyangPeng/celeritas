#include "source_proto_generator.h"
#include "common/logger.h"

#include <boost/algorithm/string.hpp>
#include <boost/filesystem/path.hpp>

#include <fstream>

celeritas::source_proto_generator::source_proto_generator(const std::string_view message_full_name,
                                                          const std::string_view message_name,
                                                          const std::string_view one_of_name,
                                                          const field_type& field,
                                                          std::string output_directory,
                                                          const handler_template_file& handler_template_file)
    : base_type{ message_full_name, message_name, std::move(output_directory), handler_template_file },
      one_of_name_{ one_of_name },
      field_{ field }
{
}

void celeritas::source_proto_generator::execute()
{
    auto message_handler_cpp_content = get_message_handler_cpp_content();

    const auto payload_name = get_payload_name();
    const auto proto_full_name = get_proto_full_name();
    const auto message_name = get_message_name();

    const auto add_handler_function = get_add_handler_function(payload_name, proto_full_name, message_name);

    boost::replace_all(message_handler_cpp_content, "${proto_name}", message_name);
    boost::replace_all(message_handler_cpp_content, "${add_handler_function}", add_handler_function);

    boost::filesystem::path path{ get_output_directory() };
    path = path / (message_name + "_message_handler.cpp");

    if (const auto file_name = path.string();
        !is_content_same(file_name, message_handler_cpp_content))
    {
        save_handler(file_name, message_handler_cpp_content);
    }
}

std::string celeritas::source_proto_generator::get_payload_name() const
{
    auto payload_name = one_of_name_;
    if (!payload_name.empty())
    {
        payload_name[0] = std::toupper(payload_name[0]);
    }

    return payload_name;
}

std::string celeritas::source_proto_generator::get_add_handler_function(const std::string& payload_name, const std::string& proto_full_name, const std::string& message_name) const
{
    std::string add_handler_function{};
    for (const auto& field : field_)
    {
        const auto entry_enum_name = get_entry_enum_name(field);

        auto add_handler_function_content = get_add_handler_function_content();

        boost::replace_all(add_handler_function_content, "${proto_name}", message_name);
        boost::replace_all(add_handler_function_content, "${proto_full_name}", proto_full_name);
        boost::replace_all(add_handler_function_content, "${payload_name}", payload_name);
        boost::replace_all(add_handler_function_content, "${entry_enum_name}", entry_enum_name);
        boost::replace_all(add_handler_function_content, "${entry_name}", field);

        add_handler_function += add_handler_function_content;
    }

    return add_handler_function;
}

std::string celeritas::source_proto_generator::get_entry_enum_name(const std::string& field)
{
    auto entry_enum_name = field;
    std::vector<std::string> parts{};

    boost::split(parts, entry_enum_name, boost::is_any_of("_"));

    for (auto& part : parts)
    {
        if (!part.empty())
        {
            part[0] = std::toupper(part[0]);
        }
    }

    return boost::join(parts, "");
}