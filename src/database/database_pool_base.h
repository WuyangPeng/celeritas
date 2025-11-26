#pragma once

#include "detail/cleanup_database_session_timer.h"

#include <boost/asio/awaitable.hpp>

namespace celeritas
{
    class database_pool_base : public std::enable_shared_from_this<database_pool_base>
    {
    public:
        using class_type = database_pool_base;
        using io_context_type = boost::asio::io_context;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using bool_awaitable_type = boost::asio::awaitable<bool>;
        using error_code_type = boost::system::error_code;
        using basis_database_manager_const_shared_ptr = std::shared_ptr<const database_entity_change>;
        using result_container = std::vector<database_entity_change>;
        using database_field_container = std::vector<database_field>;
        using optional_basis_database_manager = std::optional<database_entity_change>;
        using basis_database_manager_awaitable_type = boost::asio::awaitable<optional_basis_database_manager>;
        using result_container_awaitable_type = boost::asio::awaitable<result_container>;

        database_pool_base() noexcept = default;

        virtual ~database_pool_base() noexcept = default;

        database_pool_base(const database_pool_base& rhs) noexcept = delete;

        database_pool_base& operator=(const database_pool_base& rhs) noexcept = delete;

        database_pool_base(database_pool_base&& rhs) noexcept = delete;

        database_pool_base& operator=(database_pool_base&& rhs) noexcept = delete;

        [[nodiscard]] virtual void_awaitable_type async_initialize() = 0;

        void start_cleanup_timer(io_context_type& io_context);

        void stop_cleanup_timer();

        virtual void cleanup_database_by_duration() = 0;

        [[nodiscard]] virtual bool_awaitable_type is_health() = 0;

        [[nodiscard]] bool_awaitable_type execute_changes(const basis_database_manager_const_shared_ptr& database);

        [[nodiscard]] virtual bool_awaitable_type execute_changes(const basis_database_manager_const_shared_ptr& database, int expiration_time) = 0;

        [[nodiscard]] virtual basis_database_manager_awaitable_type select_one(const basis_database_manager_const_shared_ptr& database, const database_field_container& field_name_container) = 0;

        [[nodiscard]] virtual result_container_awaitable_type select_all(const basis_database_manager_const_shared_ptr& database, const database_field_container& field_name_container) = 0;

    private:
        using cleanup_database_session_timer_shared_ptr = std::shared_ptr<cleanup_database_session_timer>;

        cleanup_database_session_timer_shared_ptr cleanup_database_session_timer_;
    };
}
