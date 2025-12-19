#pragma once

#include "database/database_pool_base.h"

namespace celeritas
{
    // 用于拦截数据库调用的手动模拟数据库连接池。
    class mock_database_pool final : public database_pool_base
    {
    public:
        using class_type = mock_database_pool;
        using base_type = database_pool_base;

        // 实现基类的纯虚函数，并在其中记录调用信息
        [[nodiscard]] bool_awaitable_type execute_changes(const database_entity_change_const_shared_ptr& database, int expiration_time) override;

        [[nodiscard]] void_awaitable_type async_initialize() override;

        void cleanup_database_by_duration() override;

        [[nodiscard]] bool_awaitable_type is_health() override;

        [[nodiscard]] optional_database_entity_change_awaitable_type select_one(const database_entity_change_const_shared_ptr& database, const database_field_container& field_name_container) override;

        [[nodiscard]] result_container_awaitable_type select_all(const database_entity_change_const_shared_ptr& database, const database_field_container& field_name_container) override;

        [[nodiscard]] int get_execute_changes_call_count() const;

        [[nodiscard]] int get_select_one_call_count() const;

        void set_execute_changes_result(bool result);

        void set_select_one_result(bool result);

    private:
        [[nodiscard]] static database_entity_change select_user_time_refresh();

        [[nodiscard]] static database_entity_change select_user_role();

        [[nodiscard]] static database_entity_change select_user_server_roles();

        [[nodiscard]] static database_entity_change select_account_last_login();

        [[nodiscard]] static database_entity_change select_user_red_dots();

        [[nodiscard]] static database_entity_change select_mock_user();

        [[nodiscard]] static database_entity_change select_mock_account();

        [[nodiscard]] static database_entity_change select_mock_account_bind();

        int execute_changes_call_count_ = 0;
        int select_one_call_count_ = 0;
        bool execute_changes_result_ = true;
        bool select_one_result_ = true;
    };
}