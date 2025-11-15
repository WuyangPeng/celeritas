#include "basis_database_manager.h"
#include "database_change_type.h"
#include "database_entity.h"

celeritas::database_entity::database_entity(database_type database_type, std::string_view database_name, const basis_database_container& key)
    : entity_{ key }, modify_{ database_type, database_name, database_change_type::insert_type, key }
{
}

celeritas::database_entity::database_entity(const basis_database_manager& entity)
    : entity_{ entity.get_database() }, modify_{ entity.get_database_type(), entity.get_database_name(), database_change_type::update_type, entity.get_key() }
{
}

celeritas::basis_database_manager celeritas::database_entity::get_modify() const noexcept
{
    return modify_;
}

celeritas::basis_database_manager celeritas::database_entity::get_delete() const
{
    return basis_database_manager{ modify_.get_database_type(), modify_.get_database_name(), database_change_type::delete_type, modify_.get_key() };
}

void celeritas::database_entity::clear_modify()
{
    modify_.clear();
}

bool celeritas::database_entity::is_modify() const
{
    return modify_.is_modify();
}

void celeritas::database_entity::add_modify(const basis_database& basis_database)
{
    entity_.modify(basis_database);
    modify_.modify(basis_database);
}