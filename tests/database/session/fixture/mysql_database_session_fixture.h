#pragma once

#include "database_session_fixture.h"
#include "database/session/mysql_database_session.h"

namespace celeritas
{
    class mysql_database_session_fixture : public database_session_fixture<mysql_database_session>
    {
    public:
        using class_type = mysql_database_session_fixture;
        using bass_type = database_session_fixture;

        mysql_database_session_fixture();

        void init_session() override;

        void init_config() override;
    };
}
