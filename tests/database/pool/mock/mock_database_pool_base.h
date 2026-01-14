#pragma once

#include "database/pool/database_pool_base.h"

namespace celeritas
{
    class mock_database_pool_base final : public database_pool_base
    {
    public:
        using class_type = mock_database_pool_base;
        using base_type = database_pool_base;

        [[nodiscard]] void_awaitable_type async_initialize() override;

        void cleanup_database_by_duration() override;

        [[nodiscard]] bool_awaitable_type is_health() override;

        [[nodiscard]] bool_awaitable_type execute_changes(const const_database_entity_change_shared_ptr&, int expiration_time) override;

        [[nodiscard]] optional_database_entity_change_awaitable_type select_one(const const_database_entity_change_shared_ptr&, const database_field_container&) override;

        [[nodiscard]] result_container_awaitable_type select_all(const const_database_entity_change_shared_ptr&, const database_field_container&) override;

        [[nodiscard]] int get_cleanup_called_count() const;

        [[nodiscard]] bool is_execute_changes_called() const;

        [[nodiscard]] int get_expiration_time_passed() const;

    private:
        std::atomic_int cleanup_called_count = 0;
        bool execute_changes_called = false;
        int expiration_time_passed = -1;
    };
}