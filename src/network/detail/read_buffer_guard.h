#pragma once

#include "common/buffer/buffer_guard.h"
#include "network/message_header.h"

#include <optional>
#include <span>

namespace celeritas
{
    class read_buffer_guard
    {
    public:
        using class_type = read_buffer_guard;
        using payload_data_type = std::span<const char>;
        using buffer_guard_optional_type = std::optional<buffer_guard>;

        explicit read_buffer_guard(const payload_data_type& payload_data);

        [[nodiscard]] buffer_guard_optional_type get_buffer_guard();

        [[nodiscard]] message_header get_message_header() const;

    private:
        void set_message_header();

        const payload_data_type& payload_data_;
        message_header message_header_;
        bool is_successful_ = true;
    };
}