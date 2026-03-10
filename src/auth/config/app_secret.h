#pragma once

#include "provider_manager.h"
#include "database/generated/mysql/auth/apps.h"

namespace celeritas
{
    class app_secret : public provider_manager<apps>
    {
    public:
        using class_type = app_secret;

        [[nodiscard]] static app_secret& get_instance();

        [[nodiscard]] std::string get_key(int64_t app_id);

        [[nodiscard]] const_entity_shared_ptr get_apps(int64_t app_id);

    protected:
        [[nodiscard]] bool is_entity_active(const const_entity_shared_ptr& entity) const override;

        [[nodiscard]] key_type get_entity_key(const const_entity_shared_ptr& entity) const override;

        [[nodiscard]] std::string get_entity_name() const override;

        [[nodiscard]] std::string format_key_info(const key_type& key) const override;

    private:
        enum class app_secret_create
        {
            init,
        };

    public:
        explicit app_secret(app_secret_create type) noexcept;
    };
}
