#pragma once

namespace celeritas
{
    enum class account_type
    {
        invalid = -1,
        guest = 0,
        phone = 1,
        email = 2,
        password = 3,
        sdk = 4,
    };
}