#include "basis_database_manager.h"
#include "database_change_type.h"
#include "database_data_type.h"
#include "mongo_database_session.h"
#include "common/celeritas_error.h"
#include "common/logger.h"
#include "basis_database.tpp"
#include "database_field.h"

#include <boost/asio/use_awaitable.hpp>
#include <mongocxx/exception/operation_exception.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/lexical_cast.hpp>

celeritas::mongo_database_session::mongo_database_session(const std::string_view& host,
                                                          int port,
                                                          const std::string_view& user,
                                                          const std::string_view& password,
                                                          const std::string_view& uri,
                                                          const std::string_view& db_name,
                                                          int expire_seconds,
                                                          io_context_type& io_context)
    : client_{}, database_{}, io_context_{ io_context }, uri_{ uri }, db_name_{ db_name }
{
}

celeritas::mongo_database_session::void_awaitable_type celeritas::mongo_database_session::async_connect()
{
    co_await boost::asio::post(io_context_, boost::asio::use_awaitable);

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

celeritas::mongo_database_session::cursor_awaitable_type celeritas::mongo_database_session::async_find(const std::string_view& collection_name, const document_view_type& filter)
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
}

celeritas::database_session::bool_awaitable_type celeritas::mongo_database_session::is_health()
{
    co_await boost::asio::post(io_context_, boost::asio::use_awaitable);

    if (!database_)
    {
        co_return false;
    }

    try
    {
        bsoncxx::builder::basic::document ping_cmd{};
        ping_cmd.append(bsoncxx::builder::basic::kvp("ping", 1));

        database_->run_command(ping_cmd.view());

        co_return true;
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

celeritas::mongo_database_session::void_awaitable_type celeritas::mongo_database_session::save(const basis_database_manager_shared_ptr& database)
{
    co_await boost::asio::post(io_context_, boost::asio::use_awaitable);

    switch (database->get_change_type())
    {
        case database_change_type::select_type:
        {
            throw celeritas_error("change type is select.");
        }

        case database_change_type::update_type:
        {
            auto keyDocument = get_document(database->get_key());
            auto updateDocument = get_document(database->get_database());

            auto collection = (*database_)[database->get_database_name().data()];
            collection.update_one(keyDocument.extract(), updateDocument.extract());

            co_return;
        }
        case database_change_type::insert_type:
        {
            auto collection = (*database_)[database->get_database_name().data()];

            auto document = get_document(database->get_database());

            collection.insert_one(document.extract());

            co_return;
        }
        case database_change_type::delete_type:
        {
            auto collection = (*database_)[database->get_database_name().data()];

            auto document = get_document(database->get_key());

            collection.delete_one(document.extract());

            co_return;
        }
    }

    co_return;
}

celeritas::database_session::basis_database_manager_awaitable_type celeritas::mongo_database_session::select_one(const basis_database_manager& database, const database_field_container& field_name_container)
{
    co_await boost::asio::post(io_context_, boost::asio::use_awaitable);

    auto collection = (*database_)[database.get_database_name().data()];

    auto key_document = get_document(database.get_key());

    const auto result = collection.find_one(key_document.extract());

    basis_database_manager select{ database.get_database_type(), database.get_database_name(), database_change_type::select_type, database.get_key() };

    if (result)
    {
        for (const auto& value : result.value())
        {
            if (const auto basis_database = get_basis_database(field_name_container, value);
                basis_database.get_data_type() != database_data_type::null_type)
            {
                select.modify(basis_database);
            }
        }
    }

    co_return select;
}

celeritas::database_session::result_container_awaitable_type celeritas::mongo_database_session::select_all(const basis_database_manager& database, const database_field_container& field_name_container)
{
    co_await boost::asio::post(io_context_, boost::asio::use_awaitable);

    auto collection = (*database_)[database.get_database_name().data()];

    auto key_document = get_document(database.get_key());

    auto result = collection.find(key_document.extract());

    result_container result_container{};
    for (const auto& entity : result)
    {
        basis_database_manager select{ database.get_database_type(), database.get_database_name(), database_change_type::select_type, database.get_key() };

        for (const auto& value : entity)
        {
            if (const auto basis_database = get_basis_database(field_name_container, value);
                basis_database.get_data_type() != database_data_type::null_type)
            {
                select.modify(basis_database);
            }
        }

        result_container.emplace_back(select);
    }

    co_return result_container;
}

celeritas::mongo_database_session::cursor_awaitable_type celeritas::mongo_database_session::async_execute_query(const std::string_view& collection_name, const document_view_type& filter)
{
    co_await boost::asio::post(io_context_, boost::asio::use_awaitable);

    auto collection = (*database_)[collection_name.data()];

    auto cursor = collection.find(filter);

    co_return cursor;
}

celeritas::mongo_database_session::cursor_awaitable_type celeritas::mongo_database_session::async_handle_and_retry(const std::string_view& collection_name, const document_view_type& filter)
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
    client_ = std::make_unique<mongocxx::client>(mongocxx::uri{ uri_ });
    database_ = std::make_unique<mongocxx::database>((*client_)[db_name_]);

    bsoncxx::builder::basic::document ping_cmd{};
    ping_cmd.append(bsoncxx::builder::basic::kvp("ping", 1));

    database_->run_command(ping_cmd.view());

    LOG_CHANNEL(database_channel, info) << "MongoDB session connected to: " << uri_ << "/" << client_;

    co_return;
}

bsoncxx::builder::basic::document celeritas::mongo_database_session::get_document(const basis_database_container& container) const
{
    bsoncxx::builder::basic::document document{};

    for (const auto& value : container)
    {
        std::string fieldName{ value.get_field_name() };
        switch (value.get_data_type())
        {
            case database_data_type::string_type:
                document.append(bsoncxx::builder::basic::kvp(fieldName, value.get_value<database_data_type::string_type>()));
                break;

            case database_data_type::int32_type:
            case database_data_type::int32_count_type:
                document.append(bsoncxx::builder::basic::kvp(fieldName, value.get_value<database_data_type::int32_type>()));
                break;

            case database_data_type::int64_type:
            case database_data_type::int64_count_type:
                document.append(bsoncxx::builder::basic::kvp(fieldName, value.get_value<database_data_type::int64_type>()));
                break;

            case database_data_type::double_type:
                document.append(bsoncxx::builder::basic::kvp(fieldName, value.get_value<database_data_type::double_type>()));
                break;

            case database_data_type::bool_type:
                document.append(bsoncxx::builder::basic::kvp(fieldName, value.get_value<database_data_type::bool_type>()));
                break;

            case database_data_type::string_array_type:
                document.append(bsoncxx::builder::basic::kvp(fieldName, value.get_array_string_value<database_data_type::string_array_type>()));
                break;

            case database_data_type::int32_array_type:
                document.append(bsoncxx::builder::basic::kvp(fieldName, value.get_array_string_value<database_data_type::int32_array_type>()));
                break;

            case database_data_type::int64_array_type:
                document.append(bsoncxx::builder::basic::kvp(fieldName, value.get_array_string_value<database_data_type::int64_array_type>()));
                break;

            case database_data_type::double_array_type:
                document.append(bsoncxx::builder::basic::kvp(fieldName, value.get_array_string_value<database_data_type::double_array_type>()));
                break;

            default:
                break;
        }
    }

    return document;
}

celeritas::basis_database celeritas::mongo_database_session::get_basis_database(const database_field_container& field_name_container, const bsoncxx::document::element& row_view)
{
    const std::string key{ row_view.key() };
    const auto iter = std::ranges::find_if(field_name_container, [key](const auto& value) {
        return key == value.get_field_name();
    });

    if (iter == field_name_container.cend())
    {
        return basis_database{ "nullptr" };
    }

    switch (iter->get_data_type())
    {
        case database_data_type::string_type:
        {
            const std::string result{ row_view.get_string().value };
            return basis_database{ iter->get_field_name(), result };
        }

        case database_data_type::int32_type:
        case database_data_type::int32_count_type:
            return basis_database{ iter->get_field_name(), row_view.get_int32() };

        case database_data_type::int64_type:
        case database_data_type::int64_count_type:
            return basis_database{ iter->get_field_name(), row_view.get_int64() };

        case database_data_type::double_type:
            return basis_database{ iter->get_field_name(), row_view.get_double().value };

        case database_data_type::bool_type:
            return basis_database{ iter->get_field_name(), row_view.get_bool() };

        case database_data_type::string_array_type:
        {
            const std::string column{ row_view.get_string().value };

            basis_database::string_array element{};
            if (!column.empty())
            {
                split(element, column, boost::is_any_of("|"), boost::token_compress_off);
            }

            return basis_database{ iter->get_field_name(), element };
        }

        case database_data_type::int32_array_type:
        {
            const std::string column{ row_view.get_string().value };
            basis_database::string_array element{};
            if (!column.empty())
            {
                split(element, column, boost::is_any_of("|"), boost::token_compress_off);
            }

            basis_database::int32_array result{};
            for (const auto& value : element)
            {
                if (!value.empty())
                {
                    result.emplace_back(boost::lexical_cast<int32_t>(value));
                }
            }

            return basis_database{ iter->get_field_name(), result };
        }

        case database_data_type::int64_array_type:
        {
            const std::string column{ row_view.get_string().value };
            basis_database::string_array element{};
            if (!column.empty())
            {
                split(element, column, boost::is_any_of("|"), boost::token_compress_off);
            }

            basis_database::int64_array result{};
            for (const auto& value : element)
            {
                if (!value.empty())
                {
                    result.emplace_back(boost::lexical_cast<int64_t>(value));
                }
            }

            return basis_database{ iter->get_field_name(), result };
        }

        case database_data_type::double_array_type:
        {
            const std::string column{ row_view.get_string().value };
            basis_database::string_array element{};
            if (!column.empty())
            {
                split(element, column, boost::is_any_of("|"), boost::token_compress_off);
            }

            basis_database::double_array result{};
            for (const auto& value : element)
            {
                if (!value.empty())
                {
                    result.emplace_back(boost::lexical_cast<double>(value));
                }
            }

            return basis_database{ iter->get_field_name(), element };
        }

        default:
            return basis_database{ iter->get_field_name(), std::string{} };
    }
}