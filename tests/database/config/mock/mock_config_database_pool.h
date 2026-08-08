#pragma once

#include "database/pool/database_pool_base.h"

namespace celeritas
{
    class mock_config_database_pool final : public database_pool_base
    {
    public:
        using class_type = mock_config_database_pool;
        using base_type = database_pool_base;

        result_container_awaitable_type select_all(const const_database_entity_change_shared_ptr& database, const database_field_container& field_name_container) override;

        bool_awaitable_type execute_changes(const const_database_entity_change_shared_ptr& database, int expiration_time) override;

        void_awaitable_type async_initialize() override;

        void cleanup_database_by_duration() override;

        bool_awaitable_type is_health() override;

        optional_database_entity_change_awaitable_type select_one(const const_database_entity_change_shared_ptr& database, const database_field_container& field_name_container) override;

        result_container_awaitable_type select_page(const const_database_entity_change_shared_ptr& database,
                                                    const database_field_container& field_name_container,
                                                    const database_select_options& options) override;

        int64_awaitable_type select_count(const const_database_entity_change_shared_ptr& database) override;

        int64_awaitable_type select_count(const const_database_entity_change_shared_ptr& database,
                                          const database_select_options& options) override;

        static constexpr int64_t time_refresh_id = 1;
    };
}