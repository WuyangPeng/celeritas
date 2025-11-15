#pragma once

#include "database_entity.h"

template <typename T>
void celeritas::database_entity::add_modify(std::string_view field_name, T value)
{
    add_modify(basis_database{ field_name, value });
}