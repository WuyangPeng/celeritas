#include "importer_error_collector.h"
#include "common/logger.h"

using namespace std::literals;

void celeritas::importer_error_collector::RecordError(const absl::string_view filename, const int line, const int column, const absl::string_view message)
{
    auto error_message =
        "proto error in file: "s +
        filename.data() +
        " (line: " + std::to_string(line) +
        ", column: " + std::to_string(column) +
        "): " +
        message.data();

    LOG_CHANNEL(default_channel, error) << error_message;
}

void celeritas::importer_error_collector::RecordWarning(const absl::string_view filename, const int line, const int column, const absl::string_view message)
{
    auto warning_message =
        "proto warning in file: "s +
        filename.data() +
        " (line: " + std::to_string(line) +
        ", column: " + std::to_string(column) +
        "): " +
        message.data();

    LOG_CHANNEL(default_channel, warning) << warning_message;
}