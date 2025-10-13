#include "initializer.h"
#include "common/common_fwd.h"
#include "common/logger.h"
#include "database/database_pool_manager.h"
#include "proto/common/common.pb.h"
#include "proto/request.pb.h"
#include "server/server_fwd.h"

using namespace std::literals;

celeritas::initializer::initializer_shared_ptr celeritas::initializer::create(const std::string_view& server_type, std::string_view config_file_path)
{
    return std::make_shared<initializer>(server_type, config_file_path);
}

celeritas::initializer::initializer(const std::string_view& server_type, const std::string_view config_file_path)
    : server_type_{ server_type },
      current_path_{ boost::filesystem::current_path() },
      configuration_loader_{ initializer_factory::create_configuration_loader(server_type, config_file_path) },
      resource_loader_{ initializer_factory::create_resource_loader(server_type, configuration_loader_->get_app_config()) },
      application_loader_{ initializer_factory::create_application_loader(server_type, configuration_loader_->get_app_config()) },
      io_context_{},
      work_guard_{ boost::asio::make_work_guard(io_context_) },
      signals_{ io_context_, SIGINT, SIGTERM }
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
    LOG_CHANNEL(initializer_channel, info) << get_server_type() << " server is start";
    io_context_.run();
}

std::string celeritas::initializer::get_server_type() const
{
    auto server_type = server_type_;
    std::ranges::replace(server_type, '_', ' ');

    return server_type;
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
    resource_loader_->initialize(io_context_, shared_from_this());
}

void celeritas::initializer::initialize_application()
{
    application_loader_->initialize();
}

void celeritas::initializer::setup_signal_handler()
{
    // 异步等待信号
    signals_.async_wait(
        [this](const boost::system::error_code& error, const int signal_number) {
            if (!error)
            {
                LOG_CHANNEL(initializer_channel, info) << get_server_type() << " server is stop! signal_number = " << signal_number << ",error = " << error.message();

                io_context_.stop();
                database_pool_manager::get_instance().release_pool();
                resource_loader_->release_resource();
                application_loader_->stop();

                LOG_CHANNEL(initializer_channel, info) << get_server_type() << " server is stop finish!";
            }
        });
}

void celeritas::initializer::call_back(const message_header& message_header, buffer_guard buffer_guard)
{
    proto::header_request header_request{};

    if (!header_request.ParseFromArray(buffer_guard.get(), message_header.get_header_size()))
    {
        LOG_CHANNEL(initializer_channel, error) << "Failed to parse header_request from binary data.";
        return;
    }

    switch (header_request.payload_case())
    {
        case proto::header_request::PayloadCase::kClient:
        {
            const auto& client_header = header_request.client();
            break;
        }

        case proto::header_request::PayloadCase::kServer:
        {
            // 获取 server_message_header 对象
            const auto& server_header = header_request.server();
            break;
        }

        case proto::header_request::PayloadCase::PAYLOAD_NOT_SET:
        {
            LOG_CHANNEL(initializer_channel, error) << "消息头为空.";
            break;
        }
    }

    proto::request request{};

    if (!request.ParseFromArray(buffer_guard.get() + message_header.get_header_size(), message_header.get_body_size()))
    {
        LOG_CHANNEL(initializer_channel, error) << "Failed to parse request from binary data.";
        return;
    }

    switch (request.payload_case())
    {
        case proto::request::PayloadCase::kService:
        {
            const auto& service = request.service();
            break;
        }

        case proto::request::PayloadCase::PAYLOAD_NOT_SET:
        {
            LOG_CHANNEL(initializer_channel, error) << "消息体为空.";
            break;
        }
    }
}