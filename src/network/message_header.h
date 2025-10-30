#pragma once

#include <cstdint>
#include <span>

namespace celeritas
{
    // 一个消息头来指示消息大小和类型
    class message_header
    {
    public:
        using class_type = message_header;
        using source_type = std::span<const char>;

        message_header() noexcept = default;

        message_header(size_t header_size, size_t body_size);

        [[nodiscard]] static size_t get_self_size();

        [[nodiscard]] size_t get_total_size() const;

        [[nodiscard]] bool is_effective() const;

        [[nodiscard]] uint16_t get_header_type() const;

        [[nodiscard]] uint16_t get_header_size() const;

        [[nodiscard]] uint32_t get_body_size() const;

        void network_to_host();

        void host_to_network();

        [[nodiscard]] source_type get_span() const;

    private:
        uint16_t header_type = 0;
        uint16_t header_size = 0;
        uint32_t body_size = 0;
    };
}