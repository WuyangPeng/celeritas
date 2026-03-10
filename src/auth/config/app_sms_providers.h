#pragma once

#include "provider_manager.h"
#include "database/generated/mysql/auth/sms_providers.h"

namespace celeritas
{
    class app_sms_providers : public provider_manager<sms_providers>
    {
    public:
        using class_type = app_sms_providers;

        [[nodiscard]] static app_sms_providers& get_instance();

        [[nodiscard]] const_entity_shared_ptr get_sms_providers(int64_t provider_id);

    protected:
        [[nodiscard]] bool is_entity_active(const const_entity_shared_ptr& entity) const override;

        [[nodiscard]] key_type get_entity_key(const const_entity_shared_ptr& entity) const override;

        [[nodiscard]] std::string get_entity_name() const override;

        [[nodiscard]] std::string format_key_info(const key_type& key) const override;

    private:
        app_sms_providers() noexcept = default;
    };
}
