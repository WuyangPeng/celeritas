#include "app_email_providers.h"
#include "provider_manager.tpp"

#include <format>

celeritas::app_email_providers& celeritas::app_email_providers::get_instance()
{
    static std::shared_ptr<app_email_providers> instance{ new app_email_providers{} };

    return *instance;
}

celeritas::app_email_providers::const_entity_shared_ptr celeritas::app_email_providers::get_email_providers(const int64_t provider_id)
{
    return get_entity(provider_id);
}

bool celeritas::app_email_providers::is_entity_active(const const_entity_shared_ptr& entity) const
{
    return entity->is_active();
}

celeritas::app_email_providers::key_type celeritas::app_email_providers::get_entity_key(const const_entity_shared_ptr& entity) const
{
    return entity->get_provider_id();
}

std::string celeritas::app_email_providers::get_entity_name() const
{
    return "email provider";
}

std::string celeritas::app_email_providers::format_key_info(const key_type& key) const
{
    return std::format("provider id = {}", key);
}