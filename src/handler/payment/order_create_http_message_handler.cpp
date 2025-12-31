#include "order_create_http_message_handler.h"
#include "boost/asio/co_spawn.hpp"
#include "common/logging/logger.h"
#include "handler/handler_fwd.h"
#include "message/http_handle_parameter.h"
#include "payment/order_create.h"
#include "initializer/initializer_fwd.h"

std::string celeritas::order_create_http_message_handler::get_supported_type_name() const
{
    return order_create_path.data();
}

bool celeritas::order_create_http_message_handler::handle(const http_handle_parameter_shared_ptr& handle_parameter,
                                                          const http_message_registry_weak_ptr& message_registry)
{
    if (handle_parameter->get_server_type() != payment_type)
    {
        return false;
    }

    boost::asio::co_spawn(handle_parameter->get_io_context(),
                          response(handle_parameter),
                          boost::asio::detached);

    return true;
}

celeritas::order_create_http_message_handler::void_awaitable_type celeritas::order_create_http_message_handler::response(http_handle_parameter_shared_ptr handle_parameter)
{
    try
    {
        order_create order_create{ handle_parameter };

        co_return co_await order_create.response();
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(handler_channel, error) << "order create error: " << error.what();
    }
    catch (...)
    {
        LOG_CHANNEL(handler_channel, fatal) << "order create unknown error.";
    }

    const order_create_http_response response{ game_error_type::unknown, "unknown error" };
    co_return co_await handle_parameter->write_immediately(response.to_json_string());
}