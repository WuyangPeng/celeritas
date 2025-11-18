#include "basis_database_manager.h"
#include "database_change_type.h"

celeritas::basis_database_manager::basis_database_manager(const database_type database_type, const std::string_view database_name, const database_change_type change_type, basis_database_container key)
    : database_type_{ database_type }, database_name_{ database_name }, change_type_{ change_type }, key_{ std::move(key) }, database_{}
{
}

celeritas::basis_database_manager::basis_database_manager(const database_type database_type, const std::string_view database_name, const database_change_type change_type, basis_database_container key, basis_database_container database)
    : database_type_{ database_type }, database_name_{ database_name }, change_type_{ change_type }, key_{ std::move(key) }, database_{ std::move(database) }
{
}

celeritas::database_type celeritas::basis_database_manager::get_database_type() const noexcept
{
    return database_type_;
}

std::string_view celeritas::basis_database_manager::get_database_name() const noexcept
{
    return database_name_;
}

celeritas::database_change_type celeritas::basis_database_manager::get_change_type() const noexcept
{
    return change_type_;
}

celeritas::basis_database_container celeritas::basis_database_manager::get_key() const noexcept
{
    return key_;
}

celeritas::basis_database_container celeritas::basis_database_manager::get_database() const noexcept
{
    return database_;
}

celeritas::basis_database_manager celeritas::basis_database_manager::get_select() const
{
    return basis_database_manager{ database_type_, database_name_, database_change_type::select_type, key_ };
}

void celeritas::basis_database_manager::modify(const basis_database& basis_database)
{
    database_.modify(basis_database);
}

void celeritas::basis_database_manager::set(const basis_database_container& database)
{
    database_ = database;
}

void celeritas::basis_database_manager::clear()
{
    database_.clear();
    if (change_type_ == database_change_type::insert_type)
    {
        change_type_ = database_change_type::update_type;
    }
}

bool celeritas::basis_database_manager::is_modify() const
{
    return 0 < database_.get_size() || change_type_ == database_change_type::delete_type;
}

std::any celeritas::basis_database_manager::get_any_value(const std::string_view field_name) const
{
    return database_.get_any_value(field_name);
}