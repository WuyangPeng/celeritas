#include "initializer.h"
#include "common/celeritas_error.h"
#include "common/logger.h"
#include "server/server_fwd.h"
#include "common/common_fwd.h"

using namespace std::literals;

celeritas::initializer::initializer(const std::string_view& server_type, const std::string_view config_file_path, boost::asio::io_context& io_context)
    : config_file_path_{ config_file_path },
      current_path_{ boost::filesystem::current_path() },
      configuration_loader_{ initializer_factory::create_configuration_loader(server_type, config_file_path) },
      io_context_{ io_context },
      work_guard_{ boost::asio::make_work_guard(io_context) },
      signals_{ io_context, SIGINT, SIGTERM }
{
    setup_signal_handler();
}

void celeritas::initializer::initialize()
{
    initialize_default_logger();
    initialize_config();
    initialize_resource();
    initialize_application();
}

void celeritas::initializer::run()
{
    LOG_CHANNEL(initializer_channel, info) << "server is start";
    io_context_.run();
}


void celeritas::initializer::initialize_default_logger()
{
    logger::init_global(logger::severity_level_type::trace);
    logger::init_console(logger::severity_level_type::trace);

    const auto filename = current_path_ / logger_path / (std::string{ initializer_channel } + logger_extension.data());
    logger::init_file(initializer_channel,
                      filename.string(),
                      logger::severity_level_type::trace,
                      default_logger_rotation_size,
                      true);
}

void celeritas::initializer::initialize_config()
{
    configuration_loader_->initialize();
}

void celeritas::initializer::initialize_resource()
{
    initialize_logger_resource();
    initialize_database_resource();
    initialize_server_resource();
    initialize_health_check_url_resource();
    initialize_service_registry_resource();
}

void celeritas::initializer::initialize_logger_resource()
{
}

void celeritas::initializer::initialize_database_resource()
{
}

void celeritas::initializer::initialize_server_resource()
{
}

void celeritas::initializer::initialize_health_check_url_resource()
{
}

void celeritas::initializer::initialize_service_registry_resource()
{
}

void celeritas::initializer::initialize_application()
{
}

void celeritas::initializer::setup_signal_handler()
{
    // 异步等待信号
    signals_.async_wait(
        [this](const boost::system::error_code& error, const int signal_number) {
            if (!error)
            {
                LOG_CHANNEL(initializer_channel, info) << "server is stop! signal_number = " << signal_number << ",error = " << error.message();
                io_context_.stop();
            }
        });
}