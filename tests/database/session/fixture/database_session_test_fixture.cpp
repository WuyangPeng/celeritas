#include "database_session_test_fixture.h"
#include "config/basic/database_type.h"
#include "database/basic/database_change_type.h"
#include "database/basic/basis_database_container.h"

#include <boost/asio/detached.hpp>

celeritas::database_session_test_fixture::database_session_test_fixture()
    : io_context_{},
      session_{},
      select_change_{ std::make_shared<const database_entity_change>(database_type::unknown, "test", database_change_type::select_type) },
      update_change_{ std::make_shared<const database_entity_change>(database_type::unknown, "test", database_change_type::update_type, std::make_shared<const basis_database_container>()) },
      fields_{}
{
}

void celeritas::database_session_test_fixture::run(const awaitable_function& test_body)
{
    boost::asio::co_spawn(io_context_, test_body, boost::asio::detached);
    io_context_.run();
}

celeritas::mock_database_session& celeritas::database_session_test_fixture::get_session()
{
    return session_;
}

const celeritas::database_session_test_fixture::const_database_entity_change_shared_ptr& celeritas::database_session_test_fixture::get_select_change() const
{
    return select_change_;
}

const celeritas::database_session_test_fixture::const_database_entity_change_shared_ptr& celeritas::database_session_test_fixture::get_update_change() const
{
    return update_change_;
}

const celeritas::database_session_test_fixture::database_field_container& celeritas::database_session_test_fixture::get_fields() const
{
    return fields_;
}