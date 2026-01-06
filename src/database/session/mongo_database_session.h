#pragma once

#include "database_session.h"
#include "database/detail/mongo_parameter.h"

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

        explicit mongo_database_session(const std::string& host,
                                        int port,
                                        const std::string& user,
                                        const std::string& password,
                                        const std::string& uri,
                                        const std::string& db_name,
                                        int expire_seconds,
                                        const any_io_executor& any_io_executor);

        [[nodiscard]] void_awaitable_type async_connect();

        [[nodiscard]] cursor_awaitable_type async_find(std::string_view collection_name,
                                                       const document_view_type& filter);

        [[nodiscard]] bool_awaitable_type is_health() override;

        [[nodiscard]] void_awaitable_type execute_changes(const database_entity_change_const_shared_ptr& database,
                                                          int expiration_time) override;

        [[nodiscard]] database_entity_change_awaitable_type select_one(const database_entity_change_const_shared_ptr& database,
                                                                       const database_field_container& field_name_container) override;

        [[nodiscard]] result_container_awaitable_type select_all(const database_entity_change_const_shared_ptr& database,
                                                                 const database_field_container& field_name_container) override;

    private:
        using mongo_client_unique_ptr = std::unique_ptr<mongocxx::client>;
        using mongo_database_unique_ptr = std::unique_ptr<mongocxx::database>;
        using document_type = bsoncxx::builder::basic::document;
        using document_element_type = bsoncxx::document::element;
        using collection_type = mongocxx::collection;

        [[nodiscard]] bool do_is_health() const;

        void update_document(const database_entity_change_const_shared_ptr& database) const;

        void insert_document(const database_entity_change_const_shared_ptr& database) const;

        void delete_document(const database_entity_change_const_shared_ptr& database) const;

        [[nodiscard]] cursor_awaitable_type async_execute_query(std::string_view collection_name,
                                                                const document_view_type& filter) const;

        [[nodiscard]] cursor_awaitable_type async_handle_and_retry(std::string_view collection_name,
                                                                   const document_view_type& filter);

        [[nodiscard]] void_awaitable_type do_async_connect();

        [[nodiscard]] static database_entity_change to_database_entity_change(const database_entity_change_const_shared_ptr& database,
                                                                              const database_field_container& field_name_container,
                                                                              const document_view_type& view);

        [[nodiscard]] collection_type get_collection(std::string_view collection_name) const;

        any_io_executor any_io_executor_;
        mongo_client_unique_ptr client_;
        mongo_database_unique_ptr database_;

        mongo_parameter mongo_parameter_;
    };
}