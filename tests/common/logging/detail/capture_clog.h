#pragma once

#include <sstream>
#include <string>

namespace celeritas
{
    class capture_clog
    {
    public:
        capture_clog();

        ~capture_clog() noexcept;

        [[nodiscard]] std::string str() const;

    private:
        std::stringstream buffer;
        std::streambuf* old_buffer;
    };
}
