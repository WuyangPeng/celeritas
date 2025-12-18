#pragma once

namespace celeritas
{
    enum class red_type
    {
        null = 0,

        // 角色功能类
        role = 10000,

        // 物品功能类
        item = 20000,

        // 活动功能类
        activity = 30000,

        // 养成功能类
        develop = 40000,

        // 邮件功能类
        mail = 50000,

        // 任务功能类
        task = 60000,

        // 属性功能类
        attribute = 70000,

        // 副本功能类
        instance = 80000,
    };
}