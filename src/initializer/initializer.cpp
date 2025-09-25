#include "initializer.h"
#include "common/celeritas_error.h"
#include "common/logger.h"
#include "server/server_fwd.h"
#include "service_registry_server/service_registry_initializer.h"
#include "common/common_fwd.h"


using namespace std::literals;

celeritas::initializer::initializer(const std::string_view config_file_path, boost::asio::io_context& io_context) noexcept
    : config_file_path_{ config_file_path },
      current_path_{ boost::filesystem::current_path() },
      config_path_{ current_path_ / config_path },
      app_config_{},
      io_context_{ io_context },
      work_guard_{ boost::asio::make_work_guard(io_context) },
      signals_{ io_context, SIGINT, SIGTERM }
{
    setup_signal_handler();
}

void celeritas::initializer::initialize()
{
    initialize_config();
    initialize_resource();
    initialize_application();
}

void celeritas::initializer::run()
{
    LOG_CHANNEL(initializer_channel, info) << "server is start";
    io_context_.run();
}

celeritas::initializer::initializer_unique_ptr celeritas::initializer::create_initializer(const std::string_view& server_type, const std::string_view& config_file_path, boost::asio::io_context& io_context)
{
    if (server_type == service_registry_type)
    {
        return std::make_unique<service_registry_initializer>(config_file_path, io_context);
    }

    throw celeritas_error("unrecognized server type");
}

void celeritas::initializer::initialize_config()
{
    initialize_service_registry_config();
    initialize_server_config();
    initialize_health_check_url_config();
}

void celeritas::initializer::initialize_service_registry_config()
{
    const auto filename = config_path_ / service_registry_xml;

    app_config_.load_service_registry_config(filename.string());
}

void celeritas::initializer::initialize_server_config()
{
    const auto filename = config_path_ / config_file_path_ / server_xml;

    app_config_.load_server_config(filename.string());
}

void celeritas::initializer::initialize_health_check_url_config()
{
    const auto filename = config_path_ / config_file_path_ / health_check_url_xml;

    app_config_.load_health_check_url_config(filename.string());
}

void celeritas::initializer::setup_signal_handler()
{
    // 异步等待信号
    signals_.async_wait(
        [this](const boost::system::error_code& error, int signal_number) {
            if (!error)
            {
                LOG_CHANNEL(initializer_channel, info) << "server is stop! signal_number = " << signal_number;
                io_context_.stop();
            }
        });
}