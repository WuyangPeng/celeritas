#pragma once

#include "database/session/database_session.h"

namespace celeritas
{
    class mock_database_session final : public database_session
    {
    public:
        using class_type = mock_database_session;
        using base_type = database_session;

        mock_database_session();

        [[nodiscard]] bool_awaitable_type is_health() override;

        [[nodiscard]] void_awaitable_type execute_changes(const const_database_entity_change_shared_ptr& database, int expiration_time) override;

        [[nodiscard]] database_entity_change_awaitable_type select_one(const const_database_entity_change_shared_ptr& database, const database_field_container& field_name_container) override;

        [[nodiscard]] result_container_awaitable_type select_all(const const_database_entity_change_shared_ptr& database, const database_field_container& field_name_container) override;

        [[nodiscard]] bool get_is_health_called() const;

        [[nodiscard]] bool get_execute_changes_called() const;

        [[nodiscard]] bool get_select_one_called() const;

        [[nodiscard]] bool get_select_all_called() const;

    private:
        bool is_health_called_;
        bool execute_changes_called_;
        bool select_one_called_;
        bool select_all_called_;
    };
}
