#pragma once

#include "database/basic/database_entity_change.h"
#include "database/session/mock/mock_database_session.h"

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/io_context.hpp>

#include <functional>
#include <memory>

namespace celeritas
{
    class database_session_test_fixture
    {
    public:
        using class_type = database_session_test_fixture;
        using database_field_container = database_session::database_field_container;
        using const_database_entity_change_shared_ptr = std::shared_ptr<const database_entity_change>;
        using awaitable_function = std::function<boost::asio::awaitable<void>()>;

        database_session_test_fixture();

        void run(awaitable_function test_body);

        [[nodiscard]] mock_database_session& get_session();

        [[nodiscard]] const const_database_entity_change_shared_ptr& get_select_change() const;

        [[nodiscard]] const const_database_entity_change_shared_ptr& get_update_change() const;

        [[nodiscard]] const database_field_container& get_fields() const;

    protected:
        void set_test_end(bool test_end);

    private:
        using io_context_type = boost::asio::io_context;

        io_context_type io_context_;
        mock_database_session session_;
        const_database_entity_change_shared_ptr select_change_;
        const_database_entity_change_shared_ptr update_change_;
        database_field_container fields_;
        bool test_end_ = false;
    };
}