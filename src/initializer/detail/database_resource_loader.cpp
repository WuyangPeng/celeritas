#include "database_resource_loader.h"
#include "database/database_pool_manager.h"

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>

void celeritas::database_resource_loader::loader_database(boost::asio::io_context& io_context, const database_config& database_config)
{
    const auto pool = database_pool_manager::get_instance().create_pool(database_config.get_name(),
                                                                        database_config.get_database_type(),
                                                                        io_context,
                                                                        database_config.get_host(),
                                                                        database_config.get_port(),
                                                                        database_config.get_user(),
                                                                        database_config.get_password(),
                                                                        database_config.get_db_name(),
                                                                        database_config.get_min_connections(),
                                                                        database_config.get_max_connections(),
                                                                        database_config.get_expire_seconds());

    boost::asio::co_spawn(io_context,
                          pool->async_initialize(),
                          boost::asio::detached);
}