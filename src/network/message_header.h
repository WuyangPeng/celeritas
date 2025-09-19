#pragma once

#include "detail/network_internal_fwd.h"

#include <boost/asio/ip/tcp.hpp>
#include <cstdint>

namespace celeritas
{
    // 一个消息头来指示消息大小和类型
    class message_header
    {
    public:
        using class_type = message_header;

        [[nodiscard]] size_t get_total_size() const;
        [[nodiscard]] bool is_effective() const;
        [[nodiscard]] uint16_t get_header_type() const;
        [[nodiscard]] uint16_t get_header_size() const;
        [[nodiscard]] uint32_t get_body_size() const;

        void network_to_host();

    private:
        uint16_t header_type = 0;
        uint16_t header_size = 0;
        uint32_t body_size = 0;
    };
}