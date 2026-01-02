#include "logger_fixture.h"
#include "common/logging/logger.h"
#include "config/config_fwd.h"

celeritas::logger_fixture::logger_fixture()
{
    reset();
}

void celeritas::logger_fixture::reset()
{
    logger::init_global(boost::log::trivial::trace);
    logger::init_console(boost::log::trivial::trace);
    logger::init_file(default_channel.data(),
                      default_channel.data(),
                      severity_level_type::trace,
                      default_logger_rotation_size,
                      true);
}