#pragma once

#include "database/generated/mysql/apps.h"

#include <shared_mutex>
#include <unordered_map>

namespace celeritas
{
    class app_secret
    {
    public:
        using class_type = app_secret;

        [[nodiscard]] static app_secret& get_instance();

        [[nodiscard]] std::string get_key(int app_id);

        void start_auto_reload();

        void reload_from_db(int app_id);

    private:
        using apps_type = std::unordered_map<int, apps>;

        app_secret() noexcept = default;

        apps_type apps_;
        std::shared_mutex mutex_;
    };
}