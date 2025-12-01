// 此文件是自动生成，请勿手动修改。

#pragma once

#include "database/database_data_type_traits.h"
#include "database/database_entity.h"
#include "database/database_entity_change.h"
#include "database/database_index_type.h"
#include "database/entity.h"

namespace celeritas
{
    class sdk_providers final : public database_entity
    {
    public:
        using class_type = sdk_providers;
        using base_type = database_entity;

        explicit sdk_providers(const database_entity_change& entity);

        sdk_providers(database_type database_type, traits::param_type::int64_type sdk_id);

        [[nodiscard]] traits::int64_type get_sdk_id() const noexcept;

        [[nodiscard]] traits::int64_type get_app_id() const noexcept;

        [[nodiscard]] traits::int32_type get_process_type() const noexcept;

        [[nodiscard]] traits::string_type get_provider_name() const;

        [[nodiscard]] traits::string_type get_base_url() const;

        [[nodiscard]] traits::string_type get_api_key() const;

        [[nodiscard]] traits::string_type get_api_secret() const;

        [[nodiscard]] traits::string_type get_decryption_key() const;

        [[nodiscard]] traits::bool_type is_active() const noexcept;

        void set_sdk_id(traits::param_type::int64_type sdk_id);

        void set_app_id(traits::param_type::int64_type app_id);

        void set_process_type(traits::param_type::int32_type process_type);

        void set_provider_name(traits::param_type::string_type provider_name);

        void set_base_url(traits::param_type::string_type base_url);

        void set_api_key(traits::param_type::string_type api_key);

        void set_api_secret(traits::param_type::string_type api_secret);

        void set_decryption_key(traits::param_type::string_type decryption_key);

        void set_active(traits::param_type::bool_type active);

        [[nodiscard]] static const database_field_container& get_database_field_container();

        [[nodiscard]] static database_entity_change_const_shared_ptr get_select(database_type database_type);

        [[nodiscard]] static database_entity_change_const_shared_ptr get_select(database_type database_type, traits::param_type::int64_type sdk_id);

        [[nodiscard]] static database_entity_change_const_shared_ptr get_select(database_type database_type, const basis_database_container_const_shared_ptr& key);

        static constexpr std::string_view database_name{ "sdk_providers" };

        static constexpr std::string_view sdk_id_describe{ "sdk_id" };
        static constexpr std::string_view app_id_describe{ "app_id" };
        static constexpr std::string_view process_type_describe{ "process_type" };
        static constexpr std::string_view provider_name_describe{ "provider_name" };
        static constexpr std::string_view base_url_describe{ "base_url" };
        static constexpr std::string_view api_key_describe{ "api_key" };
        static constexpr std::string_view api_secret_describe{ "api_secret" };
        static constexpr std::string_view decryption_key_describe{ "decryption_key" };
        static constexpr std::string_view active_describe{ "active" };

    private:
        [[nodiscard]] static basis_database_container_const_shared_ptr get_key_basis_database_container(database_type database_type, traits::param_type::int64_type sdk_id);

        entity<sdk_id_describe, database_data_type::int64_type, database_index_type::key> sdk_id_;
        entity<app_id_describe, database_data_type::int64_type, database_index_type::composite_unique_index> app_id_;
        entity<process_type_describe, database_data_type::int32_type, database_index_type::composite_unique_index> process_type_;
        entity<provider_name_describe, database_data_type::string_type> provider_name_;
        entity<base_url_describe, database_data_type::string_type> base_url_;
        entity<api_key_describe, database_data_type::string_type> api_key_;
        entity<api_secret_describe, database_data_type::string_type> api_secret_;
        entity<decryption_key_describe, database_data_type::string_type> decryption_key_;
        entity<active_describe, database_data_type::bool_type> active_;
    };
}