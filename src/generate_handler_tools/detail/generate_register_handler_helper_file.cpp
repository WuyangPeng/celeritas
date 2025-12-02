#include "generate_register_handler_helper_file.h"
#include "register_handler_template_file.h"

#include <boost/algorithm/string.hpp>

#include <filesystem>

celeritas::generate_register_handler_helper_file::generate_register_handler_helper_file(std::string handler_directory, std::string output_directory, std::string template_directory, handler_name handler_file, handler_name http_handler_file)
    : handler_directory_{ std::move(handler_directory) }, output_directory_{ std::move(output_directory) }, template_directory_{ std::move(template_directory) }, handler_file_{ std::move(handler_file) }, http_handler_file_{ std::move(http_handler_file) }
{
}

void celeritas::generate_register_handler_helper_file::execute()
{
    const register_handler_template_file register_handler_template_file{ template_directory_ };
    auto register_handler_helper_cpp_content = register_handler_template_file.get_register_handler_helper_cpp_content();

    std::string content{};

    for (const auto& file : handler_file_)
    {
        auto file_path_boost = file;
        boost::replace_all(file_path_boost, ".h", "");

        content += "    message_registry_->register_handler(std::make_shared<" + file_path_boost + ">());\n";
    }

    boost::replace_all(register_handler_helper_cpp_content, "${register}", content);

    std::string http_content{};

    for (const auto& file : http_handler_file_)
    {
        auto file_path_boost = file;
        boost::replace_all(file_path_boost, ".h", "");

        http_content += "    http_message_registry_->register_handler(std::make_shared<" + file_path_boost + ">());\n";
    }

    boost::replace_all(register_handler_helper_cpp_content, "${http_register}", http_content);

    std::filesystem::path path{ output_directory_ };
    path = path / "register_handler_helper.cpp";

    if (const auto file_name = path.string();
        !is_content_same(file_name, register_handler_helper_cpp_content))
    {
        save_handler(file_name, register_handler_helper_cpp_content);
    }
}