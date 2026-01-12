#include "database_entity_fixture.h"

celeritas::database_entity_fixture::database_entity_fixture()
    : key_{ std::make_shared<basis_database_container>(basis_database{ "id", 1 }) },
      db_type_{ database_type::mysql },
      db_name_{ "test_db" }
{
}

celeritas::database_entity_fixture::basis_database_container_shared_ptr celeritas::database_entity_fixture::get_key() const
{
    return key_;
}

celeritas::database_type celeritas::database_entity_fixture::get_db_type() const
{
    return db_type_;
}

std::string_view celeritas::database_entity_fixture::get_db_name() const
{
    return db_name_;
}