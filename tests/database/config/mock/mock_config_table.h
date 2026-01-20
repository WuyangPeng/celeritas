#pragma once

#include "database/config/config_table_base.h"

#include <string_view>

namespace celeritas
{
    class mock_config_table : public config_table_base
    {
    public:
        mock_config_table();

        [[nodiscard]] std::string_view get_name() const override;

        [[nodiscard]] void_awaitable_type load_all(const database_pool_shared_ptr& pool) override;

        [[nodiscard]] void_awaitable_type load_one(const database_pool_shared_ptr& pool, int64_t id) override;

        [[nodiscard]] bool was_load_all_called() const;

        [[nodiscard]] bool was_load_one_called() const;

        void clear();

    private:
        std::string_view name_;
        bool load_all_called_;
        bool load_one_called_;
    };
}
