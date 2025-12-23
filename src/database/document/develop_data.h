#pragma once

#include <cstdint>
#include <string_view>

namespace celeritas
{
    class develop_data
    {
    public:
        using class_type = develop_data;

        [[nodiscard]] int get_system_id() const;

        void set_system_id(int systemId);

        [[nodiscard]] int64_t get_instance_id() const;

        void set_instance_id(int64_t instanceId);

        [[nodiscard]] int get_level() const;

        void set_level(int level);

        [[nodiscard]] int64_t get_exp() const;

        void set_exp(int64_t exp);

        [[nodiscard]] int64_t get_updated_time() const;

        void set_updated_time(int64_t updatedTime);

        [[nodiscard]] std::string to_json_string() const;

        [[nodiscard]] static develop_data from_json_string(const std::string& json_string);

        static constexpr std::string_view system_id_description = "system_id";
        static constexpr std::string_view instance_id_description = "instance_id";
        static constexpr std::string_view level_description = "level";
        static constexpr std::string_view exp_description = "exp";
        static constexpr std::string_view updated_time_description = "updated_time";

    private:
        int system_id_ = 0;
        int64_t instance_id_ = 0;
        int level_ = 0;
        int64_t exp_ = 0;
        int64_t updated_time_ = 0;
    };
}