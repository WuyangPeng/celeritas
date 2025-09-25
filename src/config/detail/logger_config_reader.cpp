#include "logger_config_reader.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

celeritas::logger_config_reader::logger_config_reader(std::string filename)
    : filename_{ std::move(filename) }, logger_{}, logger_level_{}
{
}

celeritas::logger_level_config celeritas::logger_config_reader::get_logger_level_config() const
{
    return logger_level_;
}

celeritas::logger_config_reader::logger_config_container celeritas::logger_config_reader::get_logger_config_container() const
{
    return logger_;
}

void celeritas::logger_config_reader::load_config()
{
    boost::property_tree::ptree tree{};

    boost::property_tree::xml_parser::read_xml(filename_, tree);

    for (const auto& [name , node] : tree.get_child("loggers"))
    {
        if (name == "logger")
        {
            load_node(node);
        }
        else if (name == "global_level")
        {
            logger_level_.set_default_level(node.)
        }
        else if (name == "console_level")
        {
        }
    }
}

void celeritas::logger_config_reader::load_node(const boost::property_tree::basic_ptree<std::string, std::string>& node)
{
}