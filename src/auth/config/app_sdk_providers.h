#pragma once

#include "provider_manager.h"
#include "sdk_providers_key.h"
#include "database/generated/mysql/auth/sdk_providers.h"

namespace celeritas
{
    class app_sdk_providers : public provider_manager<sdk_providers, sdk_providers_key>
    {
    public:
        using class_type = app_sdk_providers;

        [[nodiscard]] static app_sdk_providers& get_instance();

        [[nodiscard]] const_entity_shared_ptr get_sdk_providers(const sdk_providers_key& sdk_providers_key);

    protected:
        [[nodiscard]] bool is_entity_active(const const_entity_shared_ptr& entity) const override;

        [[nodiscard]] key_type get_entity_key(const const_entity_shared_ptr& entity) const override;

        [[nodiscard]] std::string get_entity_name() const override;

        [[nodiscard]] std::string format_key_info(const key_type& key) const override;

        void before_add_entity(entity_container& container, const const_entity_shared_ptr& entity) override;

    private:
        enum class app_sdk_providers_create
        {
            init,
        };

    public:
        explicit app_sdk_providers(app_sdk_providers_create type) noexcept;
    };
}