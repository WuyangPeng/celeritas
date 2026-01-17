#pragma once

#include "config/local/database_config.h"
#include "config/basic/database_type.h"
#include "database/pool/database_pool_manager.h"

#include <boost/asio/awaitable.hpp>
#include <boost/asio/io_context.hpp>

#include <functional>
#include <memory>

namespace celeritas
{
    template <typename SessionType>
    class database_session_fixture
    {
    public:
        using class_type = database_session_fixture;
        using session_shared_ptr = std::shared_ptr<SessionType>;
        using awaitable_function = std::function<boost::asio::awaitable<void>()>;
        using const_database_config_shared_ptr = std::shared_ptr<const database_config>;
        using io_context_type = boost::asio::io_context;

        virtual ~database_session_fixture() noexcept = default;

        void run(awaitable_function func);

        [[nodiscard]] session_shared_ptr get_session() const;

        [[nodiscard]] const_database_config_shared_ptr get_config() const;

    protected:
        void set_test_end(bool test_end);

        [[nodiscard]] bool is_test_end() const;

        virtual void do_pre_init();

        virtual void init_session() = 0;

        void init();

        virtual void init_config() = 0;

        void do_init_config(database_type database_type);

        [[nodiscard]] io_context_type& get_io_context();

        void set_session(session_shared_ptr session);

    private:
        void do_init();

        io_context_type io_context_;
        session_shared_ptr session_;
        const_database_config_shared_ptr config_;
        bool test_end_ = false;
    };
}
