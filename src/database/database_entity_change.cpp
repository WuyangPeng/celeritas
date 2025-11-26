#include "database_change_type.h"
#include "database_entity_change.h"

celeritas::database_entity_change::database_entity_change(const database_type database_type,
                                                          const std::string_view database_name,
                                                          const database_change_type change_type,
                                                          basis_database_container_const_shared_ptr key)
    : database_type_{ database_type },
      database_name_{ database_name },
      change_type_{ change_type },
      key_{ std::move(key) },
      database_{}
{
}

celeritas::database_entity_change::database_entity_change(const database_type database_type,
                                                          const std::string_view database_name,
                                                          const database_change_type change_type)
    : database_type_{ database_type },
      database_name_{ database_name },
      change_type_{ change_type },
      key_{ std::make_shared<basis_database_container>() },
      database_{}
{
}

celeritas::database_type celeritas::database_entity_change::get_database_type() const noexcept
{
    return database_type_;
}

std::string_view celeritas::database_entity_change::get_database_name() const noexcept
{
    return database_name_;
}

celeritas::database_change_type celeritas::database_entity_change::get_change_type() const noexcept
{
    return change_type_;
}

celeritas::database_entity_change::basis_database_container_const_shared_ptr celeritas::database_entity_change::get_key() const noexcept
{
    return key_;
}

celeritas::database_entity_change::basis_database_container_const_shared_ptr celeritas::database_entity_change::get_database() const noexcept
{
    return database_;
}

celeritas::database_entity_change celeritas::database_entity_change::get_select() const
{
    return database_entity_change{ database_type_, database_name_, database_change_type::select_type, key_ };
}

celeritas::database_entity_change celeritas::database_entity_change::get_select(const basis_database_container_const_shared_ptr& key) const
{
    return database_entity_change{ database_type_, database_name_, database_change_type::select_type, key };
}

void celeritas::database_entity_change::modify(const basis_database& basis_database)
{
    database_->modify(basis_database);
}

void celeritas::database_entity_change::clear()
{
    database_->clear();
    if (change_type_ == database_change_type::insert_type)
    {
        change_type_ = database_change_type::update_type;
    }
}

bool celeritas::database_entity_change::is_modify() const
{
    return 0 < database_->get_size() || change_type_ == database_change_type::delete_type;
}

std::any celeritas::database_entity_change::get_any_value(const std::string_view field_name) const
{
    return database_->get_any_value(field_name);
}