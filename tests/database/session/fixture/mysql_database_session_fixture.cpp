#include "database_session_fixture.tpp"
#include "mysql_database_session_fixture.h"
#include "database/session/mysql_database_session.h"

celeritas::mysql_database_session_fixture::mysql_database_session_fixture()
{
    init();
}

void celeritas::mysql_database_session_fixture::init_session()
{
    const auto config = get_config();
    set_session(std::make_shared<mysql_database_session>(config->get_host(),
                                                         config->get_port(),
                                                         config->get_user(),
                                                         config->get_password(),
                                                         "",
                                                         config->get_db_name(),
                                                         config->get_expire_seconds(),
                                                         get_io_context().get_executor()));
}

void celeritas::mysql_database_session_fixture::init_config()
{
    do_init_config(database_type::mysql);
}

