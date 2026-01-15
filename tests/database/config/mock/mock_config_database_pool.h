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

        bool_awaitable_type execute_changes(const const_database_entity_change_shared_ptr&, int) override;

        void_awaitable_type async_initialize() override;

        void cleanup_database_by_duration() override;

        bool_awaitable_type is_health() override;

        optional_database_entity_change_awaitable_type select_one(const const_database_entity_change_shared_ptr&, const database_field_container&) override;
    };
}