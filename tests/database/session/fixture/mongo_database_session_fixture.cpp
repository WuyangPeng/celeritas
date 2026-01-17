#include "database_session_fixture.tpp"
#include "mongo_database_session_fixture.h"
#include "database/pool/database_pool_manager.h"
#include "database/session/mongo_database_session.h"

celeritas::mongo_database_session_fixture::mongo_database_session_fixture()
{
    init();
}

void celeritas::mongo_database_session_fixture::do_pre_init()
{
    database_pool_manager::create_mongo_instance();
}

void celeritas::mongo_database_session_fixture::init_session()
{
    const auto config = get_config();
    const auto uri = "mongodb://" + config->get_user() + ":" + config->get_password() + "@" + config->get_host() + ":" + std::to_string(config->get_port()) + "/" + config->get_db_name();

    set_session(std::make_shared<mongo_database_session>(config->get_host(),
                                                         config->get_port(),
                                                         config->get_user(),
                                                         config->get_password(),
                                                         uri,
                                                         config->get_db_name(),
                                                         config->get_expire_seconds(),
                                                         get_io_context().get_executor()));
}

void celeritas::mongo_database_session_fixture::init_config()
{
    do_init_config(database_type::mongo);
}

