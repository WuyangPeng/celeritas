#include "consumable_data.h"

celeritas::consumable_data::consumable_data()
    : expire_time_{ 0 }
{
}

celeritas::consumable_data::consumable_data(const int64_t expire_time)
    : expire_time_{ expire_time }
{
}

int64_t celeritas::consumable_data::get_expire_time() const
{
    return expire_time_;
}

void celeritas::consumable_data::set_expire_time(const int64_t expire_time)
{
    expire_time_ = expire_time;
}