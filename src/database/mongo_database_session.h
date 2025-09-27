#pragma once

#include "database_session.h"

#include <boost/asio/awaitable.hpp>
#include <boost/asio/io_context.hpp>
#include <mongocxx/client.hpp>

namespace celeritas
{
    class mongo_database_session : public database_session
    {
    public:
        using class_type = mongo_database_session;
        using awaitable_type = boost::asio::awaitable<void>;

        explicit mongo_database_session(const std::string_view& uri,
                                        const std::string_view& db_name,
                                        boost::asio::io_context& io_context);

        ~mongo_database_session() noexcept override = default;

        mongo_database_session(const mongo_database_session& rhs) noexcept = delete;

        mongo_database_session& operator=(const mongo_database_session& rhs) noexcept = delete;

        mongo_database_session(mongo_database_session&& rhs) noexcept = delete;

        mongo_database_session& operator=(mongo_database_session&& rhs) noexcept = delete;

        [[nodiscard]] awaitable_type async_connect();

    private:
        using mongo_client_unique_ptr = std::unique_ptr<mongocxx::client>;
        using mongo_database_unique_ptr = std::unique_ptr<mongocxx::database>;

        mongo_client_unique_ptr client_;
        mongo_database_unique_ptr database_;
        boost::asio::io_context& io_context_;

        std::string uri_;
        std::string db_name_;
    };
}
