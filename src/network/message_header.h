#pragma once

#include <cstdint>

namespace celeritas
{
    // 一个消息头来指示消息大小
    struct message_header
    {
        using class_type = message_header;

        uint16_t header_type;
        uint16_t header_size;
        uint32_t body_size;
    };
}