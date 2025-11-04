#pragma once

#include "database_session.h"

#include <boost/asio/awaitable.hpp>
#include <boost/asio/io_context.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <mongocxx/client.hpp>

namespace celeritas
{
    class mongo_database_session : public database_session
    {
    public:
        using class_type = mongo_database_session;
        using base_type = database_session;
        using io_context_type = boost::asio::io_context;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using document_awaitable_type = boost::asio::awaitable<std::optional<bsoncxx::document::value> >;
        using cursor_awaitable_type = boost::asio::awaitable<mongocxx::cursor>;
        using document_view_type = bsoncxx::document::view;

        explicit mongo_database_session(const std::string_view& host,
                                        int port,
                                        const std::string_view& user,
                                        const std::string_view& password,
                                        const std::string_view& uri,
                                        const std::string_view& db_name,
                                        io_context_type& io_context);

        ~mongo_database_session() noexcept override = default;

        mongo_database_session(const mongo_database_session& rhs) noexcept = delete;

        mongo_database_session& operator=(const mongo_database_session& rhs) noexcept = delete;

        mongo_database_session(mongo_database_session&& rhs) noexcept = delete;

        mongo_database_session& operator=(mongo_database_session&& rhs) noexcept = delete;

        [[nodiscard]] void_awaitable_type async_connect();

        [[nodiscard]] cursor_awaitable_type async_find(const std::string_view& collection_name, const document_view_type& filter);

        [[nodiscard]] bool_awaitable_type is_health() override;

    private:
        using mongo_client_unique_ptr = std::unique_ptr<mongocxx::client>;
        using mongo_database_unique_ptr = std::unique_ptr<mongocxx::database>;

        [[nodiscard]] cursor_awaitable_type async_execute_query(const std::string_view& collection_name, const document_view_type& filter);

        [[nodiscard]] cursor_awaitable_type async_handle_and_retry(const std::string_view& collection_name, const document_view_type& filter);

        [[nodiscard]] void_awaitable_type do_async_connect();

        mongo_client_unique_ptr client_;
        mongo_database_unique_ptr database_;
        io_context_type& io_context_;

        std::string uri_;
        std::string db_name_;
    };
}
