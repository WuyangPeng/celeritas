#pragma once

#include "config_table_base.h"
#include "database/generated/mysql/config/time_refresh.h"

#include <optional>
#include <shared_mutex>
#include <unordered_map>

namespace celeritas
{
    class time_refresh_table : public config_table_base
    {
    public:
        using class_type = time_refresh_table;
        using base_type = config_table_base;
        using const_time_refresh_shared_ptr = std::shared_ptr<const time_refresh>;
        using optional_const_time_refresh_shared_ptr = std::optional<const_time_refresh_shared_ptr>;

        time_refresh_table();

        [[nodiscard]] const std::string& get_name() const override;

        [[nodiscard]] void_awaitable_type load_all(const database_pool_shared_ptr& pool) override;

        [[nodiscard]] void_awaitable_type load_one(const database_pool_shared_ptr& pool, int64_t id) override;

        [[nodiscard]] optional_const_time_refresh_shared_ptr get_time_refresh(int64_t id);

    private:
        using time_refresh_container_type = std::unordered_map<int64_t, const_time_refresh_shared_ptr>;

        std::string name_;
        time_refresh_container_type container_;
        std::shared_mutex shared_mutex_;
    };
}
