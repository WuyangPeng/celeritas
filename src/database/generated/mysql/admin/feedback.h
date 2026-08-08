// 此文件是自动生成，请勿手动修改。

#pragma once

#include "database/basic/database_data_type_traits.h"
#include "database/basic/database_index_type.h"
#include "database/entity/database_entity.h"
#include "database/entity/entity.h"

namespace celeritas
{
    class feedback final : public database_entity
    {
    public:
        using class_type = feedback;
        using base_type = database_entity;

        explicit feedback(const database_entity_change& entity);

        feedback(database_type database_type, const database_entity_change& entity);

        feedback(database_type database_type, traits::param_type::int64_type id);

        [[nodiscard]] traits::int64_type get_id() const noexcept;

        [[nodiscard]] traits::int64_type get_app_id() const noexcept;

        [[nodiscard]] traits::int32_type get_type() const noexcept;

        [[nodiscard]] traits::string_type get_content() const;

        [[nodiscard]] traits::string_type get_device_info() const;

        [[nodiscard]] traits::string_type get_server() const;

        [[nodiscard]] traits::string_type get_player_id() const;

        [[nodiscard]] traits::bool_type is_is_anonymous() const noexcept;

        [[nodiscard]] traits::byte_array_type get_image_data() const;

        [[nodiscard]] traits::int64_type get_create_time() const noexcept;

        void set_id(traits::param_type::int64_type id);

        void set_app_id(traits::param_type::int64_type app_id);

        void set_type(traits::param_type::int32_type type);

        void set_content(traits::param_type::string_type content);

        void set_device_info(traits::param_type::string_type device_info);

        void set_server(traits::param_type::string_type server);

        void set_player_id(traits::param_type::string_type player_id);

        void set_is_anonymous(traits::param_type::bool_type is_anonymous);

        void set_image_data(traits::param_type::byte_array_type image_data);

        void set_create_time(traits::param_type::int64_type create_time);

        void set_image_data(int index, traits::param_type::byte_array_element_type image_data);

        void add_image_data(traits::param_type::byte_array_element_type image_data);

        void remove_image_data(int index);

        [[nodiscard]] static const database_field_container& get_database_field_container();

        [[nodiscard]] static const_database_entity_change_shared_ptr get_select(database_type database_type);

        [[nodiscard]] static const_database_entity_change_shared_ptr get_select(database_type database_type, traits::param_type::int64_type id);

        [[nodiscard]] static const_database_entity_change_shared_ptr get_select(database_type database_type, const const_basis_database_container_shared_ptr& key);

        static constexpr std::string_view database_name{ "feedback" };

        static constexpr std::string_view id_describe{ "id" };
        static constexpr std::string_view app_id_describe{ "app_id" };
        static constexpr std::string_view type_describe{ "type" };
        static constexpr std::string_view content_describe{ "content" };
        static constexpr std::string_view device_info_describe{ "device_info" };
        static constexpr std::string_view server_describe{ "server" };
        static constexpr std::string_view player_id_describe{ "player_id" };
        static constexpr std::string_view is_anonymous_describe{ "is_anonymous" };
        static constexpr std::string_view image_data_describe{ "image_data" };
        static constexpr std::string_view create_time_describe{ "create_time" };

    private:
        [[nodiscard]] static const_basis_database_container_shared_ptr get_key_basis_database_container(traits::param_type::int64_type id);

        entity<id_describe, database_data_type::int64_type, database_index_type::key> id_;
        entity<app_id_describe, database_data_type::int64_type, database_index_type::composite_index> app_id_;
        entity<type_describe, database_data_type::int32_type, database_index_type::composite_index> type_;
        entity<content_describe, database_data_type::string_type> content_;
        entity<device_info_describe, database_data_type::string_type> device_info_;
        entity<server_describe, database_data_type::string_type> server_;
        entity<player_id_describe, database_data_type::string_type> player_id_;
        entity<is_anonymous_describe, database_data_type::bool_type> is_anonymous_;
        entity<image_data_describe, database_data_type::byte_array_type> image_data_;
        entity<create_time_describe, database_data_type::int64_type> create_time_;
    };
}