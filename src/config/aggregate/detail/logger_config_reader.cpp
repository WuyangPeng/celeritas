#include "logger_config_reader.h"

#include <boost/property_tree/xml_parser.hpp>

celeritas::logger_config_reader::logger_config_reader(std::string filename)
    : filename_{ std::move(filename) },
      logger_{ std::make_shared<logger_container>() },
      logger_level_{ std::make_shared<logger_level_config>() }
{
    load_config();
}

celeritas::logger_config_reader::const_logger_level_shared_ptr celeritas::logger_config_reader::get_logger_level_config() const
{
    return logger_level_;
}

celeritas::logger_config_reader::const_container_shared_ptr celeritas::logger_config_reader::get_logger_config_container() const
{
    return logger_;
}

void celeritas::logger_config_reader::load_config()
{
    boost::property_tree::ptree tree{};

    boost::property_tree::xml_parser::read_xml(filename_, tree);

    for (const auto& [name , node] : tree.get_child("loggers"))
    {
        load_node(name, node);
    }
}

void celeritas::logger_config_reader::load_node(const std::string& name, const node_type& node)
{
    if (name == "logger")
    {
        load_node(node);
    }
    else if (name == "global_level")
    {
        if (const auto global_level = node.get<std::string>("", "");
            !global_level.empty())
        {
            logger_level_->set_default_level(logger_config::get_severity_level_type(global_level));
        }
    }
    else if (name == "console_level")
    {
        if (const auto console_level = node.get<std::string>("", "");
            !console_level.empty())
        {
            logger_level_->set_console_level(logger_config::get_severity_level_type(console_level));
        }
    }
}

void celeritas::logger_config_reader::load_node(const node_type& node)
{
    // 必需配置项
    const auto name = node.get<std::string>("<xmlattr>.name");
    const auto level_name = node.get<std::string>("level");
    const auto severity_level = logger_config::get_severity_level_type(level_name);
    const auto channel_name = node.get<std::string>("channel_name");
    const auto log_file_name = node.get<std::string>("log_file_name");

    // 可选配置项
    const auto console_enabled = node.get<bool>("console_enabled", true);
    const auto rotation_size = node.get<int>("rotation_size", default_logger_rotation_size);

    logger_->emplace_back(std::make_shared<logger_config>(name, severity_level, console_enabled, channel_name, log_file_name, rotation_size));
}