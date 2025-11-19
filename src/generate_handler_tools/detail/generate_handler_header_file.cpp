#include "generate_handler_header_file.h"
#include "header_template_file.h"

#include <boost/algorithm/string.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

celeritas::generate_handler_header_file::generate_handler_header_file(std::string handler_directory, std::string output_directory, std::string template_directory, handler_file handler_file)
    : handler_directory_{ std::move(handler_directory) }, output_directory_{ std::move(output_directory) }, template_directory_{ std::move(template_directory) }, handler_file_{ std::move(handler_file) }
{
}

void celeritas::generate_handler_header_file::execute()
{
    const header_template_file header_template_file{ template_directory_ };
    auto handler_generated_h_content = header_template_file.get_handler_generated_h_content();

    std::string content{};

    const boost::filesystem::path handler_directory_path{ handler_directory_ };

    auto parent_path = handler_directory_path.parent_path();

    for (const auto& file : handler_file_)
    {
        auto file_path_boost = boost::filesystem::relative(file, parent_path).string();

        std::ranges::replace(file_path_boost, '\\', '/');

        content += "#include \"" + file_path_boost + "\"\n";
    }

    boost::replace_all(handler_generated_h_content, "${include_header}", content);

    boost::filesystem::path path{ output_directory_ };
    path = path / "handler_generated.h";

    if (const auto file_name = path.string();
        !is_content_same(file_name, handler_generated_h_content))
    {
        save_handler(file_name, handler_generated_h_content);
    }
}