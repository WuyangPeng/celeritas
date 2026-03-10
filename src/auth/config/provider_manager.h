#pragma once

#include "database/basic/database_entity_change.h"

#include <boost/asio.hpp>

#include <memory>
#include <shared_mutex>
#include <unordered_map>

namespace celeritas
{
    template <typename EntityType, typename KeyType = int64_t>
    class provider_manager : public std::enable_shared_from_this<provider_manager<EntityType, KeyType> >
    {
    public:
        using entity_type = EntityType;
        using key_type = KeyType;
        using any_io_executor = boost::asio::any_io_executor;
        using const_entity_shared_ptr = std::shared_ptr<const EntityType>;

        provider_manager() noexcept = default;

        virtual ~provider_manager() noexcept = default;

        [[nodiscard]] const_entity_shared_ptr get_entity(const key_type& key);

        void reload_from_db(const any_io_executor& any_io_executor, int64_t id);

        void load_from_db(const any_io_executor& any_io_executor);

    protected:
        using optional_database_entity_change = std::optional<database_entity_change>;
        using database_entity_change_container = std::vector<database_entity_change>;
        using entity_container = std::unordered_map<key_type, const_entity_shared_ptr>;
        using void_awaitable_type = boost::asio::awaitable<void>;

        [[nodiscard]] virtual bool is_entity_active(const const_entity_shared_ptr& entity) const = 0;

        [[nodiscard]] virtual key_type get_entity_key(const const_entity_shared_ptr& entity) const = 0;

        [[nodiscard]] virtual std::string get_entity_name() const = 0;

        [[nodiscard]] virtual std::string format_key_info(const key_type& key) const = 0;

        [[nodiscard]] virtual std::string_view get_database_name() const;

        virtual void on_entity_loaded(const const_entity_shared_ptr& entity);

        virtual void before_add_entity(entity_container& container, const const_entity_shared_ptr& entity);

        [[nodiscard]] void_awaitable_type load_all_from_db();

        [[nodiscard]] void_awaitable_type load_one_from_db(int64_t id);

        void add_entity(const optional_database_entity_change& optional_entity);

        [[nodiscard]] entity_container build_entity_container(const database_entity_change_container& result);

    private:
        entity_container entities_;
        std::shared_mutex mutex_;
    };
}
