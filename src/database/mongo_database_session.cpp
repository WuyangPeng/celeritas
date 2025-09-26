#include "mongo_database_session.h"
#include "common/logger.h"
#include "common/common_fwd.h"

celeritas::mongo_database_session::mongo_database_session(boost::asio::io_context& io_context)
    : client_{}, io_context_{ io_context }, uri_{}, db_name_{}
{
}

celeritas::mongo_database_session::~mongo_database_session() noexcept
{
}