#include "app_secret.h"
#include "common/celeritas_error.h"

celeritas::app_secret& celeritas::app_secret::get_instance()
{
    static app_secret instance{};

    return instance;
}

std::string celeritas::app_secret::get_key(int app_id)
{
    std::shared_lock lock{ mutex_ };

    if (const auto iter = apps_.find(app_id);
        iter != apps_.cend())
    {
        return iter->second.get_app_secret();
    }

    throw celeritas_error("app_secret not registered");
}

void celeritas::app_secret::start_auto_reload()
{
}

void celeritas::app_secret::reload_from_db(int app_id)
{
}