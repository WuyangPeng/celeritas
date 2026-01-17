#include "database_session_test_fixture.h"
#include "common/common_constant.h"
#include "common/core/noexcept_safe_call_and_log.h"
#include "config/basic/database_type.h"
#include "database/basic/basis_database_container.h"
#include "database/basic/database_change_type.h"

#include <boost/asio/detached.hpp>
#include <boost/test/unit_test.hpp>

celeritas::database_session_test_fixture::database_session_test_fixture()
    : io_context_{},
      session_{ io_context_.get_executor() },
      select_change_{ std::make_shared<const database_entity_change>(database_type::unknown,
                                                                     "test",
                                                                     database_change_type::select_type) },
      update_change_{ std::make_shared<const database_entity_change>(database_type::unknown,
                                                                     "test",
                                                                     database_change_type::update_type,
                                                                     std::make_shared<const basis_database_container>(basis_database{ "id", 1 })) },
      fields_{}
{
}

void celeritas::database_session_test_fixture::run(awaitable_function test_body)
{
    boost::asio::co_spawn(io_context_,
                          noexcept_safe_call_and_log_awaitable(std::move(test_body),
                                                               database_channel,
                                                               "database session test run error: "),
                          boost::asio::detached);

    io_context_.run();
    io_context_.restart();

    BOOST_CHECK(test_end_);
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

void celeritas::database_session_test_fixture::set_test_end(const bool test_end)
{
    this->test_end_ = test_end;
}