// 创建时间：2026-07-25
// 修改时间：2026-07-29
// 审核时间：2026-07-29

#pragma once

#include "network/core/http_parameter.h"

namespace celeritas
{
    class admin_parameter : public http_parameter
    {
    public:
        using class_type = admin_parameter;
        using base_type = http_parameter;
        using base_type::base_type;

    protected:
        void init();

        virtual void init_required_parameter();

        virtual void init_optional_parameter();

        virtual void verify_sign();
    };
}
