#include "mysql_database_session.h"
#include "common/core/celeritas_error.h"
#include "common/core/noexcept_safe_call_and_log.h"
#include "common/logging/logger.h"
#include "database/basic/basis_database.tpp"
#include "database/basic/database_change_type.h"
#include "database/basic/database_entity_change.h"
#include "detail/mysql_row_data_converter.h"
#include "detail/mysql_statement_generator.h"

#include <boost/lexical_cast.hpp>
#include <boost/polymorphic_pointer_cast.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/asio/use_awaitable.hpp>

#include <ranges>

using namespace std::literals;

celeritas::mysql_database_session::mysql_database_session(const std::string& host,
                                                          const int port,
                                                          const std::string& user,
                                                          const std::string& password,
                                                          const std::string& uri,
                                                          const std::string& db_name,
                                                          int expire_seconds,
                                                          const any_io_executor& any_io_executor,
                                                          ssl_io_context_type* ssl_context)
    : base_type{ any_io_executor },
      connection_{ get_any_connection(get_any_io_executor(), ssl_context) },
      mysql_parameter_{ host, port, user, password, db_name }

{
}

celeritas::mysql_database_session::~mysql_database_session() noexcept
{
    noexcept_safe_call_and_log([this] {
                                   connection_.close();
                               },
                               database_channel,
                               "unexpected error during mysql connection close: ");
}

celeritas::mysql_database_session::void_awaitable_type celeritas::mysql_database_session::async_connect()
{
    co_await connection_.async_connect(mysql_parameter_.get_connect_params_type(), boost::asio::use_awaitable);

    co_return;
}

celeritas::mysql_database_session::results_awaitable_type celeritas::mysql_database_session::async_query(const std::string& sql)
{
    std::optional<error_code_type> retry_error{};

    try
    {
        co_return co_await async_execute_query(sql);
    }
    catch (const boost::system::system_error& error)
    {
        LOG_CHANNEL(database_channel, error) << "async query exception" << error.what();

        retry_error = error.code();
    }
    catch (...)
    {
        LOG_CHANNEL(database_channel, fatal) << "async_query unknown exception";
        throw;
    }

    if (retry_error.has_value())
    {
        co_await async_reconnect_on_disconnection(retry_error.value());

        co_return co_await async_execute_query(sql);
    }

    throw celeritas_error{ "mysql async query unknown exception" };
}

celeritas::database_session::bool_awaitable_type celeritas::mysql_database_session::is_health()
{
    co_return co_await noexcept_safe_call_and_log_awaitable([self = boost::polymorphic_pointer_downcast<class_type>(shared_from_this())]() -> boost::asio::awaitable<bool> {
                                                                co_await self->connection_.async_ping(boost::asio::use_awaitable);

                                                                co_return true;
                                                            },
                                                            database_channel,
                                                            "mysql health check failed with error: ",
                                                            false);
}

celeritas::mysql_database_session::void_awaitable_type celeritas::mysql_database_session::execute_changes(const const_database_entity_change_shared_ptr& database, int expiration_time)
{
    switch (database->get_change_type())
    {
        case database_change_type::select_type:
        {
            throw celeritas_error("change type is select.");
        }

        case database_change_type::update_type:
        {
            if (database->is_modify())
            {
                co_await async_query(mysql_statement_generator::generate_update_statement(database));
            }

            co_return;
        }
        case database_change_type::insert_type:
        {
            co_await async_query(mysql_statement_generator::generate_insert_statement(database));
            co_return;
        }
        case database_change_type::delete_type:
        {
            co_await async_query(mysql_statement_generator::generate_delete_statement(database));
            co_return;
        }
    }

    co_return;
}

celeritas::database_session::database_entity_change_awaitable_type celeritas::mysql_database_session::select_one(const const_database_entity_change_shared_ptr& database, const database_field_container& field_name_container)
{
    const auto result = co_await async_query(mysql_statement_generator::generate_select_statement(field_name_container, database) + " LIMIT 1;");

    if (const auto& rows = result.rows();
        !rows.empty())
    {
        co_return populate_database_from_row(database, field_name_container, rows.at(0));
    }

    co_return std::nullopt;
}

celeritas::database_session::result_container_awaitable_type celeritas::mysql_database_session::select_all(const const_database_entity_change_shared_ptr& database, const database_field_container& field_name_container)
{
    const auto result = co_await async_query(mysql_statement_generator::generate_select_statement(field_name_container, database) + ";");

    result_container container{};

    for (const auto& entity : result.rows())
    {
        container.emplace_back(populate_database_from_row(database, field_name_container, entity));
    }

    co_return container;
}

celeritas::mysql_database_session::connection_type celeritas::mysql_database_session::get_any_connection(const any_io_executor& any_io_executor, ssl_io_context_type* ssl_context)
{
    if (ssl_context == nullptr)
    {
        return { any_io_executor };
    }

    boost::mysql::any_connection_params any_connection_params{};
    any_connection_params.ssl_context = ssl_context;

    return { any_io_executor, any_connection_params };
}

celeritas::mysql_database_session::results_awaitable_type celeritas::mysql_database_session::async_execute_query(const std::string& sql)
{
    results_type results{};

    co_await connection_.async_execute(sql, results, boost::asio::use_awaitable);

    co_return results;
}

celeritas::mysql_database_session::void_awaitable_type celeritas::mysql_database_session::async_reconnect_on_disconnection(const error_code_type& error_code)
{
    if (error_code == boost::asio::error::eof ||
        error_code == boost::asio::error::broken_pipe ||
        error_code == boost::asio::error::connection_reset)
    {
        LOG_CHANNEL(database_channel, warning) << "Database connection lost. Trying to reconnect...";

        try
        {
            co_await async_connect();

            LOG_CHANNEL(database_channel, info) << "Database reconnected successfully. Retrying query.";
        }
        catch (const std::exception& reconnect_error)
        {
            LOG_CHANNEL(database_channel, error) << "Reconnection failed: " << reconnect_error.what();

            throw;
        }
    }

    throw celeritas_error("async query exception.");
}

celeritas::database_entity_change celeritas::mysql_database_session::populate_database_from_row(const const_database_entity_change_shared_ptr& database, const database_field_container& field_name_container, const row_view_type& row)
{
    auto select = database->get_select();
    auto index = 0;

    for (const auto& field_view : row)
    {
        select.modify(mysql_row_data_converter::get_basis_database(field_name_container.at(index), field_view));
        ++index;
    }
    return select;
}
