#include "importer_error_collector.h"
#include "common/logger/logger.h"

using namespace std::literals;

void celeritas::importer_error_collector::RecordError(const absl::string_view filename, const int line, const int column, const absl::string_view message)
{
    LOG_CHANNEL(default_channel, warning) << get_message(filename, line, column, message, "error");
}

void celeritas::importer_error_collector::RecordWarning(const absl::string_view filename, const int line, const int column, const absl::string_view message)
{
    LOG_CHANNEL(default_channel, warning) << get_message(filename, line, column, message, "warning");
}

std::string celeritas::importer_error_collector::get_message(const absl::string_view filename, const int line, const int column, const absl::string_view message, absl::string_view logger_level) const
{
    return "proto "s +
           logger_level.data() +
           " in file: "s +
           filename.data() +
           " (line: " + std::to_string(line) +
           ", column: " + std::to_string(column) +
           "): " +
           message.data();
}