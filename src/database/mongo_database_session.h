#pragma once

#include "database_session.h"

#include <boost/asio/awaitable.hpp>
#include <boost/asio/io_context.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <mongocxx/client.hpp>

namespace celeritas
{
    class mongo_database_session final : public database_session
    {
    public:
        using class_type = mongo_database_session;
        using base_type = database_session;
        using io_context_type = boost::asio::io_context;
        using optional_document_value = std::optional<bsoncxx::document::value>;
        using document_awaitable_type = boost::asio::awaitable<optional_document_value>;
        using cursor_awaitable_type = boost::asio::awaitable<mongocxx::cursor>;
        using document_view_type = bsoncxx::document::view;

        explicit mongo_database_session(const std::string_view& host,
                                        int port,
                                        const std::string_view& user,
                                        const std::string_view& password,
                                        const std::string_view& uri,
                                        const std::string_view& db_name,
                                        int expire_seconds,
                                        io_context_type& io_context);

        [[nodiscard]] void_awaitable_type async_connect();

        [[nodiscard]] cursor_awaitable_type async_find(const std::string_view& collection_name, const document_view_type& filter);

        [[nodiscard]] bool_awaitable_type is_health() override;

        [[nodiscard]] void_awaitable_type execute_changes(const basis_database_manager_const_shared_ptr& database) override;

        [[nodiscard]] basis_database_manager_awaitable_type select_one(const basis_database_manager_const_shared_ptr& database, const database_field_container& field_name_container) override;

        [[nodiscard]] result_container_awaitable_type select_all(const basis_database_manager_const_shared_ptr& database, const database_field_container& field_name_container) override;

    private:
        using mongo_client_unique_ptr = std::unique_ptr<mongocxx::client>;
        using mongo_database_unique_ptr = std::unique_ptr<mongocxx::database>;
        using document_type = bsoncxx::builder::basic::document;
        using document_element_type = bsoncxx::document::element;

        [[nodiscard]] cursor_awaitable_type async_execute_query(const std::string_view& collection_name, const document_view_type& filter) const;

        [[nodiscard]] cursor_awaitable_type async_handle_and_retry(const std::string_view& collection_name, const document_view_type& filter);

        [[nodiscard]] void_awaitable_type do_async_connect();

        [[nodiscard]] static document_type get_document(const basis_database_container& container);

        [[nodiscard]] static basis_database get_basis_database(const database_field_container& field_name_container, const document_element_type& row_view);

        mongo_client_unique_ptr client_;
        mongo_database_unique_ptr database_;
        io_context_type& io_context_;

        std::string uri_;
        std::string db_name_;
    };
}
