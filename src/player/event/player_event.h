#pragma once

#include "player_event_type.h"

#include <any>
#include <cstdint>
#include <string>
#include <unordered_map>

namespace celeritas
{
    // 事件对象
    class player_event
    {
    public:
        using class_type = player_event;

        explicit player_event(player_event_type type);

        [[nodiscard]] player_event_type get_type() const noexcept;

        [[nodiscard]] int64_t get_timestamp() const noexcept;

        template <typename T>
        void set_data(const std::string& key, T&& value)
        {
            data_[key] = std::forward<T>(value);
        }

        template <typename T>
        [[nodiscard]] T get_data(const std::string& key) const
        {
            if (const auto iter = data_.find(key);
                iter != data_.cend())
            {
                return std::any_cast<T>(iter->second);
            }
            return T{};
        }

        [[nodiscard]] bool has_data(const std::string& key) const;

    private:
        using container_type = std::unordered_map<std::string, std::any>;
        player_event_type type_;
        int64_t timestamp_;
        container_type data_;
    };
}
