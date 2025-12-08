#pragma once

#include "auth/auth_fwd.h"

#include <cstdint>
#include <functional>

namespace celeritas {
    class sdk_providers_key {
    public:
        using class_type = sdk_providers_key;

        sdk_providers_key(int64_t app_id, sdk_process_type sdk_process_type);

        [[nodiscard]] int64_t get_app_id() const;

        [[nodiscard]] sdk_process_type get_sdk_process_type() const;

    private:
        int64_t app_id_;
        sdk_process_type sdk_process_type_;
    };

    [[nodiscard]] bool operator==(const sdk_providers_key &lhs, const sdk_providers_key &rhs);
}

template<>
struct std::hash<celeritas::sdk_providers_key> {
    [[nodiscard]] size_t operator()(const celeritas::sdk_providers_key &key) const noexcept;
};