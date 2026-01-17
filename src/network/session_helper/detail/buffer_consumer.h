#pragma once

#include <boost/beast/core/flat_buffer.hpp>

namespace celeritas
{
    class buffer_consumer
    {
    public:
        using class_type = buffer_consumer;
        using flat_buffer_type = boost::beast::flat_buffer;

        explicit buffer_consumer(flat_buffer_type& buffer) noexcept;

        ~buffer_consumer() noexcept;

        buffer_consumer(const buffer_consumer& rhs) = delete;

        buffer_consumer& operator=(const buffer_consumer& rhs) = delete;

        buffer_consumer(buffer_consumer&& rhs) noexcept = delete;

        buffer_consumer& operator=(buffer_consumer&& rhs) noexcept = delete;

    private:
        flat_buffer_type& buffer_;
    };
}