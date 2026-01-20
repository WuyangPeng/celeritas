#pragma once

#include "config_table_base.h"
#include "database/pool/database_pool_base.h"

#include <memory>
#include <optional>
#include <shared_mutex>
#include <unordered_map>

namespace celeritas
{
    template <typename T>
    class config_table : public config_table_base
    {
    public:
        using class_type = config_table;
        using base_type = config_table_base;
        using entity_type = T;
        using const_entity_shared_ptr = std::shared_ptr<const entity_type>;
        using optional_const_entity_shared_ptr = std::optional<const_entity_shared_ptr>;

        explicit config_table(std::string_view name);

        [[nodiscard]] std::string_view get_name() const override;

        [[nodiscard]] void_awaitable_type load_all(const database_pool_shared_ptr& pool) override;

        [[nodiscard]] void_awaitable_type load_one(const database_pool_shared_ptr& pool, int64_t id) override;

        [[nodiscard]] optional_const_entity_shared_ptr get_item(int64_t id);

        void clear() override;

    private:
        using container_type = std::unordered_map<int64_t, const_entity_shared_ptr>;

        std::string_view name_;
        container_type container_;
    };
}

