#include "database_change_type.h"
#include "database_entity_change.h"
#include "common/core/celeritas_error.h"

#include <algorithm>

celeritas::database_entity_change::database_entity_change(const database_type database_type,
                                                          const std::string_view database_name,
                                                          const database_change_type change_type,
                                                          const_basis_database_container_shared_ptr key)
    : database_type_{ database_type },
      database_name_{ database_name },
      change_type_{ change_type },
      key_{ std::move(key) },
      database_{ std::make_shared<basis_database_container>() }
{
}

celeritas::database_entity_change::database_entity_change(const database_type database_type,
                                                          const std::string_view database_name,
                                                          const database_change_type change_type)
    : database_type_{ database_type },
      database_name_{ database_name },
      change_type_{ change_type },
      key_{ std::make_shared<basis_database_container>() },
      database_{ std::make_shared<basis_database_container>() }
{
    if (change_type != database_change_type::select_type)
    {
        throw celeritas_error{ "The key cannot be empty when the change type is not select_type." };
    }
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

celeritas::database_entity_change::const_basis_database_container_shared_ptr celeritas::database_entity_change::get_key() const noexcept
{
    return key_;
}

celeritas::database_entity_change::const_basis_database_container_shared_ptr celeritas::database_entity_change::get_database() const noexcept
{
    return database_;
}

celeritas::database_entity_change celeritas::database_entity_change::get_select() const
{
    return database_entity_change{ database_type_, database_name_, database_change_type::select_type, key_ };
}

celeritas::database_entity_change celeritas::database_entity_change::get_select(const const_basis_database_container_shared_ptr& key) const
{
    return database_entity_change{ database_type_, database_name_, database_change_type::select_type, key };
}

void celeritas::database_entity_change::modify(const basis_database& basis_database)
{
    if (change_type_ == database_change_type::delete_type)
    {
        throw celeritas_error{ "delete type cannot be modified." };
    }

    deep_copy_if_shared();

    database_->modify(basis_database);
}

void celeritas::database_entity_change::clear()
{
    deep_copy_if_shared();

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

bool celeritas::database_entity_change::is_must_save() const
{
    if (change_type_ == database_change_type::delete_type)
    {
        return true;
    }

    if (database_->get_size() == 0)
    {
        return false;
    }

    return *key_ != *database_;
}

const celeritas::database_entity_change::value_variant& celeritas::database_entity_change::get_variant_value(const std::string_view field_name) const
{
    return database_->get_variant_value(field_name);
}

void celeritas::database_entity_change::deep_copy_if_shared()
{
    if (!database_ || database_.use_count() == 1)
    {
        return;
    }

    database_ = std::make_shared<basis_database_container>(*database_);
}