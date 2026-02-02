#pragma once

#include "database/basic/database_data_type_traits.h"
#include "../../config/game/develop_data_key.h"

#include <cstdint>

namespace celeritas
{
    class develop_data
    {
    public:
        using class_type = develop_data;
        using document_type = traits::document_type;

        develop_data() noexcept = default;

        develop_data(int system_id, int64_t instance_id);

        [[nodiscard]] int get_system_id() const;

        void set_system_id(int systemId);

        [[nodiscard]] int64_t get_instance_id() const;

        void set_instance_id(int64_t instanceId);

        [[nodiscard]] int get_level() const;

        void set_level(int level);

        void add_level();

        [[nodiscard]] int64_t get_exp() const;

        void set_exp(int64_t exp);

        [[nodiscard]] int64_t get_updated_time() const;

        void set_updated_time(int64_t updated_time);

        void clear();

        [[nodiscard]] document_type to_document_type() const;

        [[nodiscard]] static develop_data from_document(const document_type& document);

        [[nodiscard]] develop_data_key get_develop_data_key() const;

        static constexpr std::string_view system_id_description = "system_id";
        static constexpr std::string_view instance_id_description = "instance_id";
        static constexpr std::string_view level_description = "level";
        static constexpr std::string_view exp_description = "exp";
        static constexpr std::string_view updated_time_description = "updated_time";

        static constexpr auto default_level = 0;

    private:
        int system_id_ = 0;
        int64_t instance_id_ = 0;
        int level_ = default_level;
        int64_t exp_ = 0;
        int64_t updated_time_ = 0;
    };
}