#pragma once

#include "detail/cleanup_database_session_timer.h"

#include <boost/asio/awaitable.hpp>

namespace celeritas
{
    class database_pool_base : public std::enable_shared_from_this<database_pool_base>
    {
    public:
        using class_type = database_pool_base;
        using any_io_executor = boost::asio::any_io_executor;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using bool_awaitable_type = boost::asio::awaitable<bool>;
        using error_code_type = boost::system::error_code;
        using const_database_entity_change_shared_ptr = std::shared_ptr<const database_entity_change>;
        using result_container = std::vector<database_entity_change>;
        using database_field_container = std::vector<database_field>;
        using optional_database_entity_change = std::optional<database_entity_change>;
        using optional_database_entity_change_awaitable_type = boost::asio::awaitable<optional_database_entity_change>;
        using result_container_awaitable_type = boost::asio::awaitable<result_container>;

        database_pool_base() noexcept = default;

        virtual ~database_pool_base() noexcept = default;

        database_pool_base(const database_pool_base& rhs) noexcept = delete;

        database_pool_base& operator=(const database_pool_base& rhs) noexcept = delete;

        database_pool_base(database_pool_base&& rhs) noexcept = delete;

        database_pool_base& operator=(database_pool_base&& rhs) noexcept = delete;

        [[nodiscard]] virtual void_awaitable_type async_initialize() = 0;

        void start_cleanup_timer(const any_io_executor& any_io_executor);

        void stop_cleanup_timer();

        virtual void cleanup_database_by_duration() = 0;

        [[nodiscard]] virtual bool_awaitable_type is_health() = 0;

        [[nodiscard]] bool_awaitable_type execute_changes(const const_database_entity_change_shared_ptr& database);

        [[nodiscard]] virtual bool_awaitable_type execute_changes(const const_database_entity_change_shared_ptr& database, int expiration_time) = 0;

        [[nodiscard]] virtual optional_database_entity_change_awaitable_type select_one(const const_database_entity_change_shared_ptr& database, const database_field_container& field_name_container) = 0;

        [[nodiscard]] virtual result_container_awaitable_type select_all(const const_database_entity_change_shared_ptr& database, const database_field_container& field_name_container) = 0;

    private:
        using cleanup_database_session_timer_shared_ptr = std::shared_ptr<cleanup_database_session_timer>;

        cleanup_database_session_timer_shared_ptr cleanup_database_session_timer_;
    };
}
