#pragma once

#include "database_session.h"
#include "boost/asio/io_context.hpp"

namespace celeritas
{
    class mongo_database_session : public database_session
    {
    public:
        using class_type = mongo_database_session;

        explicit mongo_database_session(boost::asio::io_context& io_context);

        ~mongo_database_session() noexcept override;

        mongo_database_session(const mongo_database_session& rhs) noexcept = default;

        mongo_database_session& operator=(const mongo_database_session& rhs) noexcept = default;

        mongo_database_session(mongo_database_session&& rhs) noexcept = default;

        mongo_database_session& operator=(mongo_database_session&& rhs) noexcept = default;

    private:
        boost::asio::io_context& io_context_;

        std::string uri_;
        std::string db_name_;
    };
}
