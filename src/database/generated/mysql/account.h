// 此文件是自动生成，请勿手动修改。

#pragma once

#include "database/database_entity.h"
#include "database/database_data_type_traits.h"
#include "database/database_index_type.h"
#include "database/entity.h"

namespace celeritas
{
    class account final : public database_entity
    {
    public:
        using class_type = account;
        using base_type = database_entity;

        [[nodiscard]] static account create(const basis_database_manager& entity, database_type database_type, traits::param_type::int64_type account_id);

        explicit account(const basis_database_manager& entity);

        account(database_type database_type, traits::param_type::int64_type account_id);

        [[nodiscard]] traits::int64_type get_account_id() const noexcept;

        [[nodiscard]] traits::string_type get_account_name() const;

        [[nodiscard]] traits::string_type get_password_hash() const;

        [[nodiscard]] traits::string_type get_salt() const;

        [[nodiscard]] traits::string_type get_device_id() const;

        [[nodiscard]] traits::int64_type get_create_time() const noexcept;

        [[nodiscard]] traits::int32_type get_status() const noexcept;

        void set_account_id(traits::param_type::int64_type account_id);

        void set_account_name(traits::param_type::string_type account_name);

        void set_password_hash(traits::param_type::string_type password_hash);

        void set_salt(traits::param_type::string_type salt);

        void set_device_id(traits::param_type::string_type device_id);

        void set_create_time(traits::param_type::int64_type create_time);

        void set_status(traits::param_type::int32_type status);

        [[nodiscard]] static const database_field_container& get_database_field_container();

        [[nodiscard]] static basis_database_manager_const_hared_ptr get_select(database_type database_type, traits::param_type::int64_type account_id);

        [[nodiscard]] static basis_database_manager_shared_ptr get_select(database_type database_type);

        [[nodiscard]] static constexpr std::string_view get_database_name()
        {
            return database_name;
        }

        static constexpr std::string_view account_id_describe{ "account_id" };
        static constexpr std::string_view account_name_describe{ "account_name" };
        static constexpr std::string_view password_hash_describe{ "password_hash" };
        static constexpr std::string_view salt_describe{ "salt" };
        static constexpr std::string_view device_id_describe{ "device_id" };
        static constexpr std::string_view create_time_describe{ "create_time" };
        static constexpr std::string_view status_describe{ "status" };

    private:
        static constexpr std::string_view database_name{ "account" };

        [[nodiscard]] static basis_database_container get_key_basis_database_container(database_type database_type, traits::param_type::int64_type account_id);

        entity<account_id_describe, database_data_type::int64_type, database_index_type::key> account_id_;
        entity<account_name_describe, database_data_type::string_type, database_index_type::unique_index> account_name_;
        entity<password_hash_describe, database_data_type::string_type> password_hash_;
        entity<salt_describe, database_data_type::string_type> salt_;
        entity<device_id_describe, database_data_type::string_type, database_index_type::unique_index> device_id_;
        entity<create_time_describe, database_data_type::int64_type> create_time_;
        entity<status_describe, database_data_type::int32_type> status_;
    };
}