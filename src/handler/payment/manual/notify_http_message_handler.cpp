#include "notify_http_message_handler.h"
#include "common/logger.h"
#include "handler/handler_fwd.h"
#include "initializer/initializer_fwd.h"
#include "message/http_handle_parameter.h"

celeritas::notify_http_message_handler::notify_http_message_handler(const int64_t sdk_id, std::string path_suffix)
    : sdk_id_{ sdk_id }, path_suffix_{ std::move(path_suffix) }
{
}

std::string celeritas::notify_http_message_handler::get_supported_type_name() const
{
    return notify_path.data() + path_suffix_;
}

bool celeritas::notify_http_message_handler::handle(const http_handle_parameter& handle_parameter, const http_message_registry_weak_ptr& message_registry)
{
    if (handle_parameter.get_server_type() != payment_type)
    {
        return false;
    }

    boost::asio::co_spawn(handle_parameter.get_io_context(),
                          response(handle_parameter),
                          boost::asio::detached);

    return true;
}

celeritas::notify_http_message_handler::void_awaitable_type celeritas::notify_http_message_handler::response(http_handle_parameter handle_parameter)
{
    try
    {
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(handler_channel, error) << "notify error: " << error.what();
    }
    catch (...)
    {
        LOG_CHANNEL(handler_channel, fatal) << "notify unknown error.";
    }

    co_return;
}