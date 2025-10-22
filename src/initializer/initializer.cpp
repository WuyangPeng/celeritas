#include "initializer.h"
#include "common/celeritas_error.h"
#include "common/logger.h"
#include "database/database_pool_manager.h"
#include "proto/request.pb.h"
#include "proto/common/common.pb.h"

#include <boost/stacktrace.hpp>
#include <csignal>

using namespace std::literals;

celeritas::initializer::initializer_shared_ptr celeritas::initializer::create(const std::string_view& server_type, std::string config_file_path)
{
    return std::make_shared<initializer>(server_type, std::move(config_file_path));
}

celeritas::initializer::initializer(const std::string_view& server_type, std::string config_file_path)
    : server_type_{ server_type },
      current_path_{ boost::filesystem::current_path() },
      configuration_loader_{ initializer_factory::create_configuration_loader(server_type, std::move(config_file_path)) },
      resource_loader_{ initializer_factory::create_resource_loader(server_type, configuration_loader_->get_app_config()) },
      application_loader_{ initializer_factory::create_application_loader(server_type, configuration_loader_->get_app_config()) },
      io_context_{},
      work_guard_{ boost::asio::make_work_guard(io_context_) },
      daemon_{ std::make_unique<daemon>(server_type) },
      signals_{ io_context_, SIGINT, SIGTERM }
{
    setup_signal_handler();

    std::vector<int> a;
    std::cout << a[1] << std::endl;
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

#ifdef WIN32

#include <windows.h>

namespace
{
    LONG WINAPI win32_crash_handler(EXCEPTION_POINTERS* ExceptionInfo)
    {
        const auto exception_code = ExceptionInfo->ExceptionRecord->ExceptionCode;
        auto signal_number = 0;

        switch (exception_code)
        {
            case EXCEPTION_ACCESS_VIOLATION: // 相当于 SIGSEGV (段错误)
            case EXCEPTION_IN_PAGE_ERROR:
            case EXCEPTION_STACK_OVERFLOW:
                signal_number = SIGSEGV;
                break;
            case EXCEPTION_ILLEGAL_INSTRUCTION: // 相当于 SIGILL (非法指令)
                signal_number = SIGILL;
                break;
            case EXCEPTION_FLT_DIVIDE_BY_ZERO: // 相当于 SIGFPE (浮点异常)
            case EXCEPTION_INT_DIVIDE_BY_ZERO:
                signal_number = SIGFPE;
                break;
            case EXCEPTION_BREAKPOINT:
            case EXCEPTION_SINGLE_STEP:
                return EXCEPTION_CONTINUE_SEARCH;
            default:
                signal_number = static_cast<int>(exception_code);
                break;
        }

        celeritas::initializer::crash_handler(signal_number);

        return EXCEPTION_EXECUTE_HANDLER;
    }
}

#endif // WIN32

void celeritas::initializer::setup_signal_handler()
{
    // 异步等待信号
    signals_.async_wait(
        [this](const boost::system::error_code& error, const int signal_number) {
            if (!error)
            {
                LOG_CHANNEL(initializer_channel, info) << get_server_type() << " server is stop! signal_number = " << signal_number << ",error = " << error.message();

                stop();
            }
        });

    #ifdef WIN32

    SetUnhandledExceptionFilter(win32_crash_handler);

    signal(SIGABRT, crash_handler);
    signal(SIGFPE, crash_handler);

    #else

    struct sigaction sa{};
    sa.sa_handler = crash_handler;

    sigemptyset(&sa.sa_mask);

    // 注册 SIGSEGV, SIGABRT, SIGFPE
    sigaction(SIGSEGV, &sa, nullptr);
    sigaction(SIGABRT, &sa, nullptr);
    sigaction(SIGFPE, &sa, nullptr);

    #endif // WIN32
}

void celeritas::initializer::stop()
{
    daemon_.reset();
    io_context_.stop();
    database_pool_manager::get_instance().release_pool();
    resource_loader_->release_resource();
    application_loader_->stop();

    LOG_CHANNEL(initializer_channel, info) << get_server_type() << " server is stop finish!";
}

void celeritas::initializer::crash_handler(const int signal_number)
{
    LOG_CHANNEL(initializer_channel, fatal) << "signal_number = " << signal_number << ".\nstack trace:\n" << boost::stacktrace::stacktrace();

    _exit(signal_number);
}

celeritas::header celeritas::initializer::get_header(const message_header& message_header, const buffer_guard& buffer_guard) const
{
    proto::common::header header_request{};

    if (!header_request.ParseFromArray(buffer_guard.get(), message_header.get_header_size()))
    {
        throw celeritas_error("Failed to parse header_request from binary data.");
    }

    switch (header_request.payload_case())
    {
        case proto::common::header::PayloadCase::kEmpty:
        {
            const auto& empty_header = header_request.empty();
            return header(empty_header);
        }

        case proto::common::header::PayloadCase::kClient:
        {
            const auto& client_header = header_request.client();
            return header(client_header);
        }

        case proto::common::header::PayloadCase::kServer:
        {
            const auto& server_header = header_request.server();
            return header(server_header);
        }

        case proto::common::header::PayloadCase::kGateway:
        {
            const auto& gateway_header = header_request.gateway();
            return header(gateway_header);
        }

        case proto::common::header::PayloadCase::kToGateway:
        {
            const auto& to_gateway_header = header_request.to_gateway();
            return header(to_gateway_header);
        }

        default:
        {
            throw celeritas_error("消息头为空.");
        }
    }
}

void celeritas::initializer::call_back(const message_header& message_header, buffer_guard buffer_guard, const session_shared_ptr& session)
{
    const auto header = get_header(message_header, buffer_guard);

    const auto request = std::make_shared<proto::request>();

    if (!request->ParseFromArray(buffer_guard.get() + message_header.get_header_size(), message_header.get_body_size()))
    {
        LOG_CHANNEL(initializer_channel, error) << "Failed to parse request from binary data.";
        return;
    }

    switch (request->payload_case())
    {
        case proto::request::PayloadCase::kService:
        {
            if (const auto& service = request->service();
                !application_loader_->dispatch(header, service, request, session, resource_loader_))
            {
                LOG_CHANNEL(initializer_channel, error) << "Failed to dispatch service request.";
            }
            break;
        }

        case proto::request::PayloadCase::PAYLOAD_NOT_SET:
        {
            LOG_CHANNEL(initializer_channel, error) << "消息体为空.";
            break;
        }
    }
}