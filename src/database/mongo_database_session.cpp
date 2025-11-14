#include "mongo_database_session.h"
#include "common/celeritas_error.h"
#include "common/logger.h"

#include <boost/asio/use_awaitable.hpp>
#include <mongocxx/exception/operation_exception.hpp>

celeritas::mongo_database_session::mongo_database_session(const std::string_view& host,
                                                          int port,
                                                          const std::string_view& user,
                                                          const std::string_view& password,
                                                          const std::string_view& uri,
                                                          const std::string_view& db_name,
                                                          int expire_seconds,
                                                          io_context_type& io_context)
    : client_{}, database_{}, io_context_{ io_context }, uri_{ uri }, db_name_{ db_name }
{
}

celeritas::mongo_database_session::void_awaitable_type celeritas::mongo_database_session::async_connect()
{
    co_await boost::asio::post(io_context_, boost::asio::use_awaitable);

    try
    {
        co_await do_async_connect();
    }
    catch (const mongocxx::exception& error)
    {
        LOG_CHANNEL(database_channel, error) << "MongoDB connection failed: " << error.what();
        throw;
    }

    co_return;
}

celeritas::mongo_database_session::cursor_awaitable_type celeritas::mongo_database_session::async_find(const std::string_view& collection_name, const document_view_type& filter)
{
    auto is_error = false;

    try
    {
        co_return co_await async_execute_query(collection_name, filter);
    }
    catch (const mongocxx::operation_exception& error)
    {
        LOG_CHANNEL(database_channel, error) << "MongoDB find failed: " << error.what();

        is_error = true;
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(database_channel, error) << "MongoDB find_one failed: " << error.what();

        is_error = true;
    }
    catch (...)
    {
        LOG_CHANNEL(database_channel, fatal) << "MongoDB find unknown exception";
        throw;
    }

    if (is_error)
    {
        co_return co_await async_handle_and_retry(collection_name, filter);
    }
}

celeritas::database_session::bool_awaitable_type celeritas::mongo_database_session::is_health()
{
    co_await boost::asio::post(io_context_, boost::asio::use_awaitable);

    if (!database_)
    {
        co_return false;
    }

    try
    {
        bsoncxx::builder::basic::document ping_cmd{};
        ping_cmd.append(bsoncxx::builder::basic::kvp("ping", 1));

        database_->run_command(ping_cmd.view());

        co_return true;
    }
    catch (const mongocxx::exception& error)
    {
        LOG_CHANNEL(database_channel, warning) << "MongoDB health check failed: " << error.what();
        co_return false;
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(database_channel, error) << "MongoDB health check failed: " << error.what();
        co_return false;
    }
    catch (...)
    {
        LOG_CHANNEL(database_channel, fatal) << "MongoDB health check unknown exception";
        co_return false;
    }
}

celeritas::mongo_database_session::cursor_awaitable_type celeritas::mongo_database_session::async_execute_query(const std::string_view& collection_name, const document_view_type& filter)
{
    co_await boost::asio::post(io_context_, boost::asio::use_awaitable);

    auto collection = (*database_)[collection_name.data()];

    auto cursor = collection.find(filter);

    co_return cursor;
}

celeritas::mongo_database_session::cursor_awaitable_type celeritas::mongo_database_session::async_handle_and_retry(const std::string_view& collection_name, const document_view_type& filter)
{
    LOG_CHANNEL(database_channel, warning) << "MongoDB connection lost. Trying to reconnect...";

    try
    {
        co_await async_connect();

        LOG_CHANNEL(database_channel, info) << "MongoDB reconnected successfully. Retrying query.";

        co_return co_await async_execute_query(collection_name, filter);
    }
    catch (const std::exception& reconnect_error)
    {
        LOG_CHANNEL(database_channel, error) << "Reconnection failed: " << reconnect_error.what();

        throw;
    }
}

celeritas::mongo_database_session::void_awaitable_type celeritas::mongo_database_session::do_async_connect()
{
    client_ = std::make_unique<mongocxx::client>(mongocxx::uri{ uri_ });
    database_ = std::make_unique<mongocxx::database>((*client_)[db_name_]);

    bsoncxx::builder::basic::document ping_cmd{};
    ping_cmd.append(bsoncxx::builder::basic::kvp("ping", 1));

    database_->run_command(ping_cmd.view());

    LOG_CHANNEL(database_channel, info) << "MongoDB session connected to: " << uri_ << "/" << client_;

    co_return;
}