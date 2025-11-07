#include "buffer_guard.h"
#include "buffer_pool.h"
#include "noexcept_safe_call_and_log.h"

celeritas::buffer_guard::buffer_guard(buffer_pool_data buffer_data)
    : buffer_data_{ std::move(buffer_data) }, effective_size_{ 0 }
{
}

celeritas::buffer_guard::buffer_guard(buffer_pool_data buffer_data, const size_t effective_size)
    : buffer_data_{ std::move(buffer_data) }, effective_size_{ effective_size }
{
}

celeritas::buffer_guard::~buffer_guard() noexcept
{
    noexcept_safe_call_and_log([this] {
                                   this->release();
                               },
                               common_channel,
                               "buffer guard release error: ");
}

celeritas::buffer_guard::buffer_guard(buffer_guard&& rhs) noexcept
    : buffer_data_{ std::move(rhs.buffer_data_) }, effective_size_{ rhs.effective_size_ }
{
}

celeritas::buffer_guard& celeritas::buffer_guard::operator=(buffer_guard&& rhs) noexcept
{
    if (this != &rhs)
    {
        buffer_data_ = std::move(rhs.buffer_data_);
        effective_size_ = rhs.effective_size_;
    }

    return *this;
}

char* celeritas::buffer_guard::get()
{
    return buffer_data_.data();
}

const char* celeritas::buffer_guard::get() const
{
    return buffer_data_.data();
}

size_t celeritas::buffer_guard::size() const
{
    return buffer_data_.size();
}

size_t celeritas::buffer_guard::get_effective_size() const
{
    return effective_size_;
}

void celeritas::buffer_guard::set_effective_size(size_t size)
{
    effective_size_ = size;
}

char* celeritas::buffer_guard::get(const size_t offset)
{
    return buffer_data_.get(offset);
}

void celeritas::buffer_guard::set(const std::string& response)
{
    buffer_data_.set(response);
}

void celeritas::buffer_guard::set(const source_type& source)
{
    buffer_data_.set(source);
}

bool celeritas::buffer_guard::is_effective() const noexcept
{
    return buffer_data_.is_effective();
}

void celeritas::buffer_guard::release()
{
    if (buffer_data_.is_effective())
    {
        buffer_pool::release(std::move(buffer_data_));
    }
}