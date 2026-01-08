#pragma once

#include "database/entity/database_entity.h"

namespace celeritas
{
    class test_entity : public database_entity
    {
    public:
        using database_entity::database_entity;
        using database_entity::add_modify;
    };
}
