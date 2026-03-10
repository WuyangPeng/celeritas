#include "app_secret.h"
#include "provider_manager.tpp"
#include "auth/core/app_status_type.h"
#include "common/core/enum_cast.h"

#include <format>

celeritas::app_secret& celeritas::app_secret::get_instance()
{
    static std::shared_ptr<app_secret> instance{ new app_secret{} };

    return *instance;
}

std::string celeritas::app_secret::get_key(const int64_t app_id)
{
    return get_apps(app_id)->get_app_secret();
}

celeritas::app_secret::const_entity_shared_ptr celeritas::app_secret::get_apps(const int64_t app_id)
{
    return get_entity(app_id);
}

bool celeritas::app_secret::is_entity_active(const const_entity_shared_ptr& entity) const
{
    return entity->get_status() != enum_cast_underlying(app_status_type::close);
}

celeritas::app_secret::key_type celeritas::app_secret::get_entity_key(const const_entity_shared_ptr& entity) const
{
    return entity->get_app_id();
}

std::string celeritas::app_secret::get_entity_name() const
{
    return "app secret";
}

std::string celeritas::app_secret::format_key_info(const key_type& key) const
{
    return std::format("app id = {}", key);
}
