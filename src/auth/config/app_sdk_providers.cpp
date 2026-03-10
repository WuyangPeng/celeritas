#include "app_sdk_providers.h"
#include "provider_manager.tpp"
#include "auth/sdk/sdk_process_type.h"
#include "common/core/enum_cast.h"

#include <format>

celeritas::app_sdk_providers& celeritas::app_sdk_providers::get_instance()
{
    static std::shared_ptr<app_sdk_providers> instance{ new app_sdk_providers{} };

    return *instance;
}

celeritas::app_sdk_providers::const_entity_shared_ptr celeritas::app_sdk_providers::get_sdk_providers(const sdk_providers_key& sdk_providers_key)
{
    return get_entity(sdk_providers_key);
}

bool celeritas::app_sdk_providers::is_entity_active(const const_entity_shared_ptr& entity) const
{
    return entity->is_active();
}

celeritas::app_sdk_providers::key_type celeritas::app_sdk_providers::get_entity_key(const const_entity_shared_ptr& entity) const
{
    return sdk_providers_key{ entity->get_app_id(), underlying_cast_enum<sdk_process_type>(entity->get_process_type()) };
}

std::string celeritas::app_sdk_providers::get_entity_name() const
{
    return "sdk provider";
}

std::string celeritas::app_sdk_providers::format_key_info(const key_type& key) const
{
    return std::format("app id = {}, sdk process type = {}", key.get_app_id(), get_sdk_process_type_description(key.get_sdk_process_type()));
}

void celeritas::app_sdk_providers::before_add_entity(entity_container& container, const const_entity_shared_ptr& entity)
{
    erase_if(container, [sdk_id = entity->get_sdk_id()](const auto& element) {
        return element.second->get_sdk_id() == sdk_id;
    });
}
