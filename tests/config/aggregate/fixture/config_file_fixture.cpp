#include "config_file_fixture.h"
#include "common/core/noexcept_safe_call_and_log.h"

#include <filesystem>
#include <fstream>

celeritas::config_file_fixture::config_file_fixture(std::string name, const std::string& content)
    : filename_{ std::move(name) }
{
    init(content);
}

celeritas::config_file_fixture::~config_file_fixture() noexcept
{
    noexcept_safe_call_and_log([this] {
                                   remove_file();
                               },
                               config_channel,
                               "remove file error: ");
}

celeritas::config_file_fixture celeritas::config_file_fixture::get_global()
{
    const std::string content{ R"(<?xml version="1.0" encoding="utf-8"?>
<global>
    <external_host>192.168.1.100</external_host>
</global>)" };

    const std::string filename{ "test_global.xml" };

    return config_file_fixture{ filename, content };
}

celeritas::config_file_fixture celeritas::config_file_fixture::get_health_check_url()
{
    const std::string content{ R"(<?xml version="1.0" encoding="utf-8"?>
<health_check_url>
    <url>/status</url>
    <interval>60</interval>
    <timeout>10</timeout>
</health_check_url>)" };
    const std::string filename{ "test_health_check.xml" };

    return config_file_fixture{ filename, content };
}

std::string celeritas::config_file_fixture::get_filename() const
{
    return filename_;
}

void celeritas::config_file_fixture::init(const std::string& content) const
{
    std::ofstream out{ filename_ };
    out << content;
    out.close();
}

void celeritas::config_file_fixture::remove_file()
{
    if (std::filesystem::exists(filename_))
    {
        std::filesystem::remove(filename_);
    }
}