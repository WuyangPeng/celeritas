#pragma once

#include "database_session_fixture.h"
#include "database/session/mongo_database_session.h"

namespace celeritas
{
    class mongo_database_session_fixture : public database_session_fixture<mongo_database_session>
    {
    public:
        using class_type = mongo_database_session_fixture;
        using bass_type = database_session_fixture;

        mongo_database_session_fixture();

        void do_pre_init() override;

        void init_session() override;

        void init_config() override;
    };
}
