#include "mongo_database_session.h"
#include "common/logger.h"
#include "common/common_fwd.h"

#include <boost/asio/use_awaitable.hpp>

celeritas::mongo_database_session::mongo_database_session(const std::string_view& uri,
                                                          const std::string_view& db_name,
                                                          boost::asio::io_context& io_context)
    : client_{}, database_{}, io_context_{ io_context }, uri_{ uri }, db_name_{ db_name }
{
}

celeritas::mongo_database_session::awaitable_type celeritas::mongo_database_session::async_connect()
{
    co_await boost::asio::post(io_context_, boost::asio::use_awaitable);

    try
    {
        client_ = std::make_unique<mongocxx::client>(mongocxx::uri{ uri_ });
        database_ = std::make_unique<mongocxx::database>((*client_)[db_name_]);

        LOG_CHANNEL(database_channel, info) << "MongoDB session connected to: " << uri_ << "/" << client_;
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(database_channel, error) << "MongoDB connection failed: " << error.what();
        throw;
    }

    co_return;
}

