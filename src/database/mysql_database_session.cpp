#include "basis_database.tpp"
#include "basis_database_manager.h"
#include "database_change_type.h"
#include "mysql_database_session.h"
#include "common/celeritas_error.h"
#include "common/logger.h"
#include "common/noexcept_safe_call_and_log.h"
#include "detail/mysql_row_data_converter.h"
#include "detail/mysql_statement_generator.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/asio/use_awaitable.hpp>

#include <ranges>

using namespace std::literals;

celeritas::mysql_database_session::mysql_database_session(const std::string_view host,
                                                          const int port,
                                                          const std::string_view user,
                                                          const std::string_view password,
                                                          const std::string_view uri,
                                                          const std::string_view db_name,
                                                          int expire_seconds,
                                                          io_context_type& io_context,
                                                          ssl_io_context_type* ssl_context)
    : connection_{ get_any_connection(io_context, ssl_context) },
      mysql_parameter_{ host, port, user, password, db_name }
{
}

celeritas::mysql_database_session::~mysql_database_session() noexcept
{
    noexcept_safe_call_and_log([this] {
                                   connection_.close();
                               },
                               common_channel,
                               "Unexpected error during MySQL connection close: ");
}

celeritas::mysql_database_session::void_awaitable_type celeritas::mysql_database_session::async_connect()
{
    co_await connection_.async_connect(mysql_parameter_.get_connect_params_type(), boost::asio::use_awaitable);

    co_return;
}

celeritas::mysql_database_session::results_awaitable_type celeritas::mysql_database_session::async_query(const std::string& sql)
{
    std::optional<boost::system::error_code> retry_error{};

    try
    {
        co_return co_await async_execute_query(sql);
    }
    catch (const boost::system::system_error& error)
    {
        LOG_CHANNEL(database_channel, error) << "async_query exception" << error.what();

        retry_error = error.code();
    }
    catch (...)
    {
        LOG_CHANNEL(database_channel, fatal) << "async_query unknown exception";
        throw;
    }

    if (retry_error.has_value())
    {
        co_return co_await async_handle_and_retry(sql, retry_error.value());
    }

    co_return results_type{};
}

celeritas::database_session::bool_awaitable_type celeritas::mysql_database_session::is_health()
{
    try
    {
        co_await connection_.async_ping(boost::asio::use_awaitable);

        co_return true;
    }
    catch (const boost::system::system_error& error)
    {
        LOG_CHANNEL(database_channel, warning) << "MySQL health check failed with error: " << error.what();
        co_return false;
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(database_channel, error) << "MySQL health check failed with unexpected exception: " << error.what();
        co_return false;
    }
    catch (...)
    {
        LOG_CHANNEL(database_channel, fatal) << "MySQL health check failed with unknown exception";
        co_return false;
    }
}

celeritas::mysql_database_session::void_awaitable_type celeritas::mysql_database_session::execute_changes(const basis_database_manager_const_shared_ptr& database)
{
    switch (database->get_change_type())
    {
        case database_change_type::select_type:
        {
            throw celeritas_error("change type is select.");
        }

        case database_change_type::update_type:
        {
            co_await async_query(mysql_statement_generator::generate_update_statement(database));
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

celeritas::database_session::basis_database_manager_awaitable_type celeritas::mysql_database_session::select_one(const basis_database_manager_const_shared_ptr& database, const database_field_container& field_name_container)
{
    const auto result = co_await async_query(mysql_statement_generator::generate_select_statement(field_name_container, *database) + " LIMIT 1;");

    if (const auto& rows = result.rows();
        !rows.empty())
    {
        co_return populate_database_from_row(database, field_name_container, rows[0]);
    }

    co_return std::nullopt;
}

celeritas::database_session::result_container_awaitable_type celeritas::mysql_database_session::select_all(const basis_database_manager_const_shared_ptr& database, const database_field_container& field_name_container)
{
    const auto result = co_await async_query(mysql_statement_generator::generate_select_statement(field_name_container, *database) + ";");

    result_container container{};

    for (const auto& entity : result.rows())
    {
        container.emplace_back(populate_database_from_row(database, field_name_container, entity));
    }

    co_return container;
}

celeritas::mysql_database_session::connection_type celeritas::mysql_database_session::get_any_connection(io_context_type& io_context, ssl_io_context_type* ssl_context)
{
    if (ssl_context == nullptr)
    {
        return { io_context };
    }

    boost::mysql::any_connection_params any_connection_params{};
    any_connection_params.ssl_context = ssl_context;

    return { io_context, any_connection_params };
}

celeritas::mysql_database_session::results_awaitable_type celeritas::mysql_database_session::async_execute_query(const std::string& sql)
{
    results_type results{};

    co_await connection_.async_execute(sql, results, boost::asio::use_awaitable);

    co_return results;
}

celeritas::mysql_database_session::results_awaitable_type celeritas::mysql_database_session::async_handle_and_retry(const std::string& sql, const error_code_type& error_code)
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

            co_return co_await async_execute_query(sql);
        }
        catch (const std::exception& reconnect_error)
        {
            LOG_CHANNEL(database_channel, error) << "Reconnection failed: " << reconnect_error.what();

            throw;
        }
    }

    throw celeritas_error("async_query exception.");
}

celeritas::basis_database_manager celeritas::mysql_database_session::populate_database_from_row(const basis_database_manager_const_shared_ptr& database, const database_field_container& field_name_container, const row_view_type& row)
{
    auto select = database->get_select();
    auto index = 0;
    for (const auto& value : row)
    {
        select.modify(mysql_row_data_converter::get_basis_database(field_name_container.at(index), value));
        ++index;
    }
    return select;
}
