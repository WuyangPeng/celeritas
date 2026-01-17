#pragma once

#include "database/session/database_session.h"

namespace celeritas
{
    class mock_pool_database_session final : public database_session
    {
    public:
        using class_type = mock_pool_database_session;
        using base_type = database_session;

        explicit mock_pool_database_session(const any_io_executor& any_io_executor);

        mock_pool_database_session(const std::string& host,
                                   int port,
                                   const std::string& user,
                                   const std::string& password,
                                   const std::string& uri,
                                   const std::string& db_name,
                                   int expire_seconds,
                                   const any_io_executor& any_io_executor);

        [[nodiscard]] void_awaitable_type async_connect();

        [[nodiscard]] bool_awaitable_type is_health() override;

        [[nodiscard]] void_awaitable_type execute_changes(const const_database_entity_change_shared_ptr& database, int expiration_time) override;

        [[nodiscard]] database_entity_change_awaitable_type select_one(const const_database_entity_change_shared_ptr& database, const database_field_container& field_name_container) override;

        [[nodiscard]] result_container_awaitable_type select_all(const const_database_entity_change_shared_ptr& database, const database_field_container& field_name_container) override;
    };
}
