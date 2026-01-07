#include "database_entity.h"
#include "database/basic/database_change_type.h"
#include "database/basic/database_entity_change.h"

celeritas::database_entity::database_entity(const database_type database_type,
                                            const std::string_view database_name,
                                            const const_basis_database_container_shared_ptr& key)
    : modify_{ std::make_shared<database_entity_change>(database_type,
                                                        database_name,
                                                        database_change_type::insert_type,
                                                        key) }
{
}

celeritas::database_entity::database_entity(const database_entity_change& entity)
    : modify_{ std::make_shared<database_entity_change>(entity.get_database_type(),
                                                        entity.get_database_name(),
                                                        database_change_type::update_type,
                                                        entity.get_key()) }
{
}

celeritas::database_entity::database_entity(database_type database_type, const database_entity_change& entity)
    : modify_{ std::make_shared<database_entity_change>(database_type,
                                                        entity.get_database_name(),
                                                        database_type == entity.get_database_type() ? database_change_type::update_type : database_change_type::insert_type,
                                                        entity.get_key())
    }
{
}

celeritas::database_entity::const_database_entity_change_shared_ptr celeritas::database_entity::get_modify() const
{
    return modify_;
}

celeritas::database_entity::const_database_entity_change_shared_ptr celeritas::database_entity::get_delete() const
{
    return std::make_shared<const database_entity_change>(modify_->get_database_type(),
                                                          modify_->get_database_name(),
                                                          database_change_type::delete_type,
                                                          modify_->get_key());
}

void celeritas::database_entity::clear_modify()
{
    modify_->clear();
}

bool celeritas::database_entity::is_modify() const
{
    return modify_->is_modify();
}

bool celeritas::database_entity::is_must_save() const
{
    return modify_->is_must_save();
}

void celeritas::database_entity::add_modify(const basis_database& basis_database)
{
    modify_->modify(basis_database);
}