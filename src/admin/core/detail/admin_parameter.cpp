// 创建时间：2026-07-29
// 修改时间：2026-07-29
// 审核时间：2026-07-29

#include "admin_parameter.h"

void celeritas::admin_parameter::init()
{
    init_required_parameter();
    if (is_failure())
    {
        return;
    }

    init_optional_parameter();
    if (is_failure())
    {
        return;
    }

    verify_sign();
}

void celeritas::admin_parameter::init_required_parameter()
{
}

void celeritas::admin_parameter::init_optional_parameter()
{
}

void celeritas::admin_parameter::verify_sign()
{
}
