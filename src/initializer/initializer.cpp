#include "initializer.h"
#include "common/celeritas_error.h"
#include "common/logger.h"
#include "server/server_fwd.h"
#include "service_registry_server/service_registry_initializer.h"
#include "common/common_fwd.h"

celeritas::initializer::initializer(const std::string_view config_file_path, boost::asio::io_context& io_context) noexcept
    : config_file_path_{ config_file_path },
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

void celeritas::initializer::setup_signal_handler()
{
    // 异步等待信号
    signals_.async_wait(
        [this](const boost::system::error_code& error, int signal_number) {
            if (!error)
            {
                LOG_CHANNEL(initializer_channel, info) << "server is stop!";
                io_context_.stop();
            }
        });
}