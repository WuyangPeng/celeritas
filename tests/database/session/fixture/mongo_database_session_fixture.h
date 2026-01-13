#pragma once

#include "config/local/database_config.h"
#include "database/session/mongo_database_session.h"

#include <boost/asio/awaitable.hpp>
#include <boost/asio/io_context.hpp>

#include <functional>
#include <memory>

namespace celeritas
{
    class mongo_database_session_fixture
    {
    public:
        using class_type = mongo_database_session_fixture;
        using awaitable_function = std::function<boost::asio::awaitable<void>()>;
        using mongo_database_session_shared_ptr = std::shared_ptr<mongo_database_session>;
        using const_database_config_shared_ptr = std::shared_ptr<const database_config>;

        mongo_database_session_fixture();

        void run(awaitable_function func);

        [[nodiscard]] mongo_database_session_shared_ptr get_session() const;

        [[nodiscard]] const_database_config_shared_ptr get_config() const;

    protected:
        void set_test_end(bool test_end);

        [[nodiscard]] bool is_test_end() const;

    private:
        using io_context_type = boost::asio::io_context;

        void init();

        void do_init();

        void init_config();

        void init_session();

        io_context_type io_context_;
        mongo_database_session_shared_ptr session_;
        const_database_config_shared_ptr config_;
        bool test_end_ = false;
    };
}


