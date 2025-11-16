#pragma once

#include "database/database_entity.h"
#include "database/database_data_type_traits.h"
#include "database/database_index_type.h"
#include "database/entity.h"

namespace celeritas
{
    class test final : public database_entity
    {
    public:
        using class_type = test;
        using base_type = database_entity;

        [[nodiscard]] static test create(const basis_database_manager& entity, database_type database_type, traits::param_type::int64_type user_id);

        explicit test(const basis_database_manager& entity);

        test(database_type database_type, traits::param_type::int64_type user_id);

        [[nodiscard]] traits::int64_type get_user_id() const noexcept;

        [[nodiscard]] traits::int32_type get_chapter_id() const noexcept;

        [[nodiscard]] traits::string_type get_chapter_name() const;

        [[nodiscard]] traits::double_type get_chance_winning() const noexcept;

        [[nodiscard]] traits::bool_type is_winning() const noexcept;

        [[nodiscard]] traits::int64_count_type get_currency() const noexcept;

        [[nodiscard]] traits::int32_count_type get_count() const noexcept;

        void set_user_id(traits::param_type::int64_type user_id);

        void set_chapter_id(traits::param_type::int32_type chapter_id);

        void set_chapter_name(traits::param_type::string_type chapter_name);

        void set_chance_winning(traits::param_type::double_type chance_winning);

        void set_winning(traits::param_type::bool_type winning);

        void set_currency(traits::param_type::int64_count_type currency);

        void set_count(traits::param_type::int32_count_type count);

        void modify_currency(traits::param_type::int64_count_type currency);

        void modify_count(traits::param_type::int32_count_type count);

        [[nodiscard]] static const database_field_container& get_database_field_container();

        [[nodiscard]] static basis_database_manager get_select(database_type database_type, traits::param_type::int64_type user_id);

        [[nodiscard]] static basis_database_manager get_select_all(database_type database_type);

        [[nodiscard]] static constexpr std::string_view get_database_name()
        {
            return database_name;
        }

    private:
        static constexpr std::string_view database_name{ "test" };

        static constexpr std::string_view user_id_describe{ "_id" };
        static constexpr std::string_view chapter_id_describe{ "chapter_id" };
        static constexpr std::string_view chapter_name_describe{ "chapter_name" };
        static constexpr std::string_view chance_winning_describe{ "chance_winning" };
        static constexpr std::string_view winning_describe{ "winning" };
        static constexpr std::string_view currency_describe{ "currency" };
        static constexpr std::string_view count_describe{ "count" };

        [[nodiscard]] static basis_database_container get_key_basis_database_container(traits::param_type::int64_type user_id);

        entity<user_id_describe, database_data_type::int64_type, database_index_type::key> user_id_;
        entity<chapter_id_describe, database_data_type::int32_type> chapter_id_;
        entity<chapter_name_describe, database_data_type::string_type> chapter_name_;
        entity<chance_winning_describe, database_data_type::double_type> chance_winning_;
        entity<winning_describe, database_data_type::bool_type> winning_;
        entity<currency_describe, database_data_type::int64_count_type> currency_;
        entity<count_describe, database_data_type::int32_count_type> count_;
    };
    ;
}