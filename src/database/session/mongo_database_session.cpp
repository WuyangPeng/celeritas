#include "mongo_database_session.h"
#include "common/core/celeritas_error.h"
#include "common/logging/logger.h"
#include "database/basic/basis_database.tpp"
#include "database/basic/database_change_type.h"
#include "database/basic/database_data_type.h"
#include "database/basic/database_entity_change.h"
#include "detail/mongo_row_data_converter.h"

#include <boost/asio/strand.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <mongocxx/exception/operation_exception.hpp>

celeritas::mongo_database_session::mongo_database_session(const std::string& host,
                                                          int port,
                                                          const std::string& user,
                                                          const std::string& password,
                                                          const std::string& uri,
                                                          const std::string& db_name,
                                                          int expire_seconds,
                                                          const any_io_executor& any_io_executor)
    : any_io_executor_{ boost::asio::make_strand(any_io_executor) },
      client_{},
      database_{},
      mongo_parameter_{ uri, db_name }
{
}

celeritas::mongo_database_session::void_awaitable_type celeritas::mongo_database_session::async_connect()
{
    co_await boost::asio::post(any_io_executor_, boost::asio::use_awaitable);

    try
    {
        co_await do_async_connect();
    }
    catch (const mongocxx::exception& error)
    {
        LOG_CHANNEL(database_channel, error) << "MongoDB connection failed: " << error.what();
        throw;
    }

    co_return;
}

celeritas::mongo_database_session::cursor_awaitable_type celeritas::mongo_database_session::async_find(const std::string_view collection_name, const document_view_type& filter)
{
    auto is_error = false;

    try
    {
        co_return co_await async_execute_query(collection_name, filter);
    }
    catch (const mongocxx::operation_exception& error)
    {
        LOG_CHANNEL(database_channel, error) << "MongoDB find failed: " << error.what();

        is_error = true;
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(database_channel, error) << "MongoDB find_one failed: " << error.what();

        is_error = true;
    }
    catch (...)
    {
        LOG_CHANNEL(database_channel, fatal) << "MongoDB find unknown exception";
        throw;
    }

    if (is_error)
    {
        co_return co_await async_handle_and_retry(collection_name, filter);
    }

    throw celeritas_error{ "mongoDB find unknown exception" };
}

celeritas::database_session::bool_awaitable_type celeritas::mongo_database_session::is_health()
{
    co_await boost::asio::post(any_io_executor_, boost::asio::use_awaitable);

    try
    {
        co_return do_is_health();
    }
    catch (const mongocxx::exception& error)
    {
        LOG_CHANNEL(database_channel, warning) << "MongoDB health check failed: " << error.what();
        co_return false;
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(database_channel, error) << "MongoDB health check failed: " << error.what();
        co_return false;
    }
    catch (...)
    {
        LOG_CHANNEL(database_channel, fatal) << "MongoDB health check unknown exception";
        co_return false;
    }
}

celeritas::mongo_database_session::void_awaitable_type celeritas::mongo_database_session::execute_changes(const const_database_entity_change_shared_ptr& database, int expiration_time)
{
    co_await boost::asio::post(any_io_executor_, boost::asio::use_awaitable);

    switch (database->get_change_type())
    {
        case database_change_type::select_type:
        {
            throw celeritas_error("change type is select.");
        }

        case database_change_type::update_type:
        {
            update_document(database);

            co_return;
        }
        case database_change_type::insert_type:
        {
            insert_document(database);

            co_return;
        }
        case database_change_type::delete_type:
        {
            delete_document(database);

            co_return;
        }
    }

    co_return;
}

celeritas::database_session::database_entity_change_awaitable_type celeritas::mongo_database_session::select_one(const const_database_entity_change_shared_ptr& database, const database_field_container& field_name_container)
{
    co_await boost::asio::post(any_io_executor_, boost::asio::use_awaitable);

    auto collection = get_collection(database->get_database_name());

    const auto key_document = mongo_row_data_converter::get_document(database->get_key());

    if (const auto result = collection.find_one(key_document->extract()))
    {
        co_return to_database_entity_change(database, field_name_container, result.value());
    }

    co_return std::nullopt;
}

celeritas::database_session::result_container_awaitable_type celeritas::mongo_database_session::select_all(const const_database_entity_change_shared_ptr& database, const database_field_container& field_name_container)
{
    co_await boost::asio::post(any_io_executor_, boost::asio::use_awaitable);

    auto collection = get_collection(database->get_database_name());

    const auto key_document = mongo_row_data_converter::get_document(database->get_key());

    auto result = collection.find(key_document->extract());

    result_container result_container{};
    for (const auto& entity : result)
    {
        result_container.emplace_back(to_database_entity_change(database, field_name_container, entity));
    }

    co_return result_container;
}

bool celeritas::mongo_database_session::do_is_health() const
{
    if (!database_)
    {
        return false;
    }

    bsoncxx::builder::basic::document ping_cmd{};
    ping_cmd.append(bsoncxx::builder::basic::kvp("ping", 1));

    database_->run_command(ping_cmd.view());

    return true;
}

void celeritas::mongo_database_session::update_document(const const_database_entity_change_shared_ptr& database) const
{
    const auto key_document = mongo_row_data_converter::get_document(database->get_key());
    const auto update_document = mongo_row_data_converter::get_document(database->get_database());

    auto collection = get_collection(database->get_database_name());
    collection.update_one(key_document->extract(), update_document->extract());
}

void celeritas::mongo_database_session::insert_document(const const_database_entity_change_shared_ptr& database) const
{
    auto collection = get_collection(database->get_database_name());

    const auto document = mongo_row_data_converter::get_document(database->get_database());

    collection.insert_one(document->extract());
}

void celeritas::mongo_database_session::delete_document(const const_database_entity_change_shared_ptr& database) const
{
    auto collection = get_collection(database->get_database_name());

    const auto document = mongo_row_data_converter::get_document(database->get_key());

    collection.delete_one(document->extract());
}

celeritas::mongo_database_session::cursor_awaitable_type celeritas::mongo_database_session::async_execute_query(const std::string_view collection_name, const document_view_type& filter) const
{
    co_await boost::asio::post(any_io_executor_, boost::asio::use_awaitable);

    auto collection = get_collection(collection_name);

    co_return collection.find(filter);
}

celeritas::mongo_database_session::cursor_awaitable_type celeritas::mongo_database_session::async_handle_and_retry(const std::string_view collection_name, const document_view_type& filter)
{
    LOG_CHANNEL(database_channel, warning) << "MongoDB connection lost. Trying to reconnect...";

    try
    {
        co_await async_connect();

        LOG_CHANNEL(database_channel, info) << "MongoDB reconnected successfully. Retrying query.";

        co_return co_await async_execute_query(collection_name, filter);
    }
    catch (const std::exception& reconnect_error)
    {
        LOG_CHANNEL(database_channel, error) << "Reconnection failed: " << reconnect_error.what();

        throw;
    }
}

celeritas::mongo_database_session::void_awaitable_type celeritas::mongo_database_session::do_async_connect()
{
    client_ = std::make_unique<mongocxx::client>(mongocxx::uri{ mongo_parameter_.get_uri() });
    database_ = std::make_unique<mongocxx::database>((*client_)[mongo_parameter_.get_db_name()]);

    document_type ping_cmd{};
    ping_cmd.append(bsoncxx::builder::basic::kvp("ping", 1));

    database_->run_command(ping_cmd.view());

    LOG_CHANNEL(database_channel, info) << "MongoDB session connected to: " << mongo_parameter_.get_uri() << "/" << mongo_parameter_.get_db_name();

    co_return;
}

celeritas::database_entity_change celeritas::mongo_database_session::to_database_entity_change(const const_database_entity_change_shared_ptr& database,
                                                                                               const database_field_container& field_name_container,
                                                                                               const document_view_type& view)
{
    auto select = database->get_select();
    for (const auto& element : view)
    {
        if (const auto basis_database = mongo_row_data_converter::get_basis_database(field_name_container, element);
            basis_database.get_data_type() != database_data_type::null_type)
        {
            select.modify(basis_database);
        }
    }
    return select;
}

celeritas::mongo_database_session::collection_type celeritas::mongo_database_session::get_collection(const std::string_view collection_name) const
{
    return (*database_)[collection_name.data()];
}