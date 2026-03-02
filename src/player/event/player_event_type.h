#pragma once

namespace celeritas
{
    // 玩家事件类型枚举
    enum class player_event_type
    {
        // 物品相关
        on_item_add = 60000,

        // 养成相关
        on_develop_level = 70000,
    };
}
