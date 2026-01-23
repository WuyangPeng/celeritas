#pragma once

#include "sdk_process_parameter.h"

#include <boost/asio/awaitable.hpp>

#include <optional>

namespace celeritas
{
    class sdk_process
    {
    public:
        using class_type = sdk_process;
        using optional_string = std::optional<std::string>;
        using optional_string_awaitable_type = boost::asio::awaitable<optional_string>;
        using sdk_process_unique_ptr = std::unique_ptr<class_type>;

        sdk_process(sdk_process_parameter sdk_process_parameter);

        virtual ~sdk_process() noexcept = default;

        sdk_process(const sdk_process& rhs) = default;

        sdk_process& operator=(const sdk_process& rhs) = default;

        sdk_process(sdk_process&& rhs) = default;

        sdk_process& operator=(sdk_process&& rhs) = default;

        [[nodiscard]] virtual optional_string_awaitable_type get_open_id() = 0;

        [[nodiscard]] static sdk_process_unique_ptr create_sdk_process(sdk_process_parameter sdk_process_parameter);

    private:
        sdk_process_parameter sdk_process_parameter_;
    };
}