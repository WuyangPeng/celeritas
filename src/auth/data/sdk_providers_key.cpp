#include "sdk_providers_key.h"

celeritas::sdk_providers_key::sdk_providers_key(const int64_t app_id, sdk_process_type sdk_process_type)
    : app_id_{ app_id }, sdk_process_type_{ sdk_process_type }
{
}

int64_t celeritas::sdk_providers_key::get_app_id() const
{
    return app_id_;
}

celeritas::sdk_process_type celeritas::sdk_providers_key::get_sdk_process_type() const
{
    return sdk_process_type_;
}

bool celeritas::operator==(const sdk_providers_key& lhs, const sdk_providers_key& rhs)
{
    return lhs.get_app_id() == rhs.get_app_id() && lhs.get_sdk_process_type() == rhs.get_sdk_process_type();
}

size_t std::hash<celeritas::sdk_providers_key>::operator()(const celeritas::sdk_providers_key& key) const noexcept
{
    size_t seed{};

    seed ^= std::hash<int64_t>()(key.get_app_id()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= std::hash<int>()(static_cast<int>(key.get_sdk_process_type())) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

    return seed;
}
