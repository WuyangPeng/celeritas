#pragma once

#include "provider_manager.h"
#include "database/generated/mysql/auth/email_providers.h"

namespace celeritas
{
    class app_email_providers : public provider_manager<email_providers>
    {
    public:
        using class_type = app_email_providers;

        [[nodiscard]] static app_email_providers& get_instance();

        [[nodiscard]] const_entity_shared_ptr get_email_providers(int64_t provider_id);

    protected:
        [[nodiscard]] bool is_entity_active(const const_entity_shared_ptr& entity) const override;

        [[nodiscard]] key_type get_entity_key(const const_entity_shared_ptr& entity) const override;

        [[nodiscard]] std::string get_entity_name() const override;

        [[nodiscard]] std::string format_key_info(const key_type& key) const override;

    private:
        app_email_providers() noexcept = default;
    };
}
