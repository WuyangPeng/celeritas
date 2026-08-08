// 此文件是自动生成，请勿手动修改。

#pragma once

#include "database/basic/database_data_type_traits.h"
#include "database/basic/database_index_type.h"
#include "database/entity/database_entity.h"
#include "database/entity/entity.h"

namespace celeritas
{
    class admin_account final : public database_entity
    {
    public:
        using class_type = admin_account;
        using base_type = database_entity;

        explicit admin_account(const database_entity_change& entity);

        admin_account(database_type database_type, const database_entity_change& entity);

        admin_account(database_type database_type, traits::param_type::int64_type account_id);

        [[nodiscard]] traits::int64_type get_account_id() const noexcept;

        [[nodiscard]] traits::string_type get_username() const;

        [[nodiscard]] traits::int64_type get_app_id() const noexcept;

        [[nodiscard]] traits::string_type get_password_hash() const;

        [[nodiscard]] traits::string_type get_salt() const;

        [[nodiscard]] traits::int32_type get_role() const noexcept;

        [[nodiscard]] traits::string_type get_nickname() const;

        [[nodiscard]] traits::string_type get_avatar() const;

        [[nodiscard]] traits::int64_type get_creator_account_id() const noexcept;

        [[nodiscard]] traits::int32_type get_status() const noexcept;

        [[nodiscard]] traits::int64_type get_create_time() const noexcept;

        [[nodiscard]] traits::int64_type get_update_time() const noexcept;

        [[nodiscard]] traits::int64_type get_last_login_time() const noexcept;

        [[nodiscard]] traits::string_type get_last_login_ip() const;

        void set_account_id(traits::param_type::int64_type account_id);

        void set_username(traits::param_type::string_type username);

        void set_app_id(traits::param_type::int64_type app_id);

        void set_password_hash(traits::param_type::string_type password_hash);

        void set_salt(traits::param_type::string_type salt);

        void set_role(traits::param_type::int32_type role);

        void set_nickname(traits::param_type::string_type nickname);

        void set_avatar(traits::param_type::string_type avatar);

        void set_creator_account_id(traits::param_type::int64_type creator_account_id);

        void set_status(traits::param_type::int32_type status);

        void set_create_time(traits::param_type::int64_type create_time);

        void set_update_time(traits::param_type::int64_type update_time);

        void set_last_login_time(traits::param_type::int64_type last_login_time);

        void set_last_login_ip(traits::param_type::string_type last_login_ip);

        [[nodiscard]] static const database_field_container& get_database_field_container();

        [[nodiscard]] static const_database_entity_change_shared_ptr get_select(database_type database_type);

        [[nodiscard]] static const_database_entity_change_shared_ptr get_select(database_type database_type, traits::param_type::int64_type account_id);

        [[nodiscard]] static const_database_entity_change_shared_ptr get_select(database_type database_type, const const_basis_database_container_shared_ptr& key);

        static constexpr std::string_view database_name{ "admin_account" };

        static constexpr std::string_view account_id_describe{ "account_id" };
        static constexpr std::string_view username_describe{ "username" };
        static constexpr std::string_view app_id_describe{ "app_id" };
        static constexpr std::string_view password_hash_describe{ "password_hash" };
        static constexpr std::string_view salt_describe{ "salt" };
        static constexpr std::string_view role_describe{ "role" };
        static constexpr std::string_view nickname_describe{ "nickname" };
        static constexpr std::string_view avatar_describe{ "avatar" };
        static constexpr std::string_view creator_account_id_describe{ "creator_account_id" };
        static constexpr std::string_view status_describe{ "status" };
        static constexpr std::string_view create_time_describe{ "create_time" };
        static constexpr std::string_view update_time_describe{ "update_time" };
        static constexpr std::string_view last_login_time_describe{ "last_login_time" };
        static constexpr std::string_view last_login_ip_describe{ "last_login_ip" };

    private:
        [[nodiscard]] static const_basis_database_container_shared_ptr get_key_basis_database_container(traits::param_type::int64_type account_id);

        entity<account_id_describe, database_data_type::int64_type, database_index_type::key> account_id_;
        entity<username_describe, database_data_type::string_type, database_index_type::composite_unique_index> username_;
        entity<app_id_describe, database_data_type::int64_type, database_index_type::composite_unique_key_and_index> app_id_;
        entity<password_hash_describe, database_data_type::string_type> password_hash_;
        entity<salt_describe, database_data_type::string_type> salt_;
        entity<role_describe, database_data_type::int32_type> role_;
        entity<nickname_describe, database_data_type::string_type> nickname_;
        entity<avatar_describe, database_data_type::string_type> avatar_;
        entity<creator_account_id_describe, database_data_type::int64_type> creator_account_id_;
        entity<status_describe, database_data_type::int32_type> status_;
        entity<create_time_describe, database_data_type::int64_type> create_time_;
        entity<update_time_describe, database_data_type::int64_type> update_time_;
        entity<last_login_time_describe, database_data_type::int64_type> last_login_time_;
        entity<last_login_ip_describe, database_data_type::string_type> last_login_ip_;
    };
}