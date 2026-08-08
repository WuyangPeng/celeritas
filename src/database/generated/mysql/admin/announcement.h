// 此文件是自动生成，请勿手动修改。

#pragma once

#include "database/basic/database_data_type_traits.h"
#include "database/basic/database_index_type.h"
#include "database/entity/database_entity.h"
#include "database/entity/entity.h"

namespace celeritas
{
    class announcement final : public database_entity
    {
    public:
        using class_type = announcement;
        using base_type = database_entity;

        explicit announcement(const database_entity_change& entity);

        announcement(database_type database_type, const database_entity_change& entity);

        announcement(database_type database_type, traits::param_type::int64_type id);

        [[nodiscard]] traits::int64_type get_id() const noexcept;

        [[nodiscard]] traits::int64_type get_app_id() const noexcept;

        [[nodiscard]] traits::string_type get_title() const;

        [[nodiscard]] traits::int32_type get_tag() const noexcept;

        [[nodiscard]] traits::string_type get_banner_url() const;

        [[nodiscard]] traits::string_type get_content() const;

        [[nodiscard]] traits::int32_type get_priority() const noexcept;

        [[nodiscard]] traits::int64_type get_publish_time() const noexcept;

        [[nodiscard]] traits::int32_type get_status() const noexcept;

        [[nodiscard]] traits::string_type get_server() const;

        [[nodiscard]] traits::int64_type get_create_time() const noexcept;

        [[nodiscard]] traits::int64_type get_update_time() const noexcept;

        void set_id(traits::param_type::int64_type id);

        void set_app_id(traits::param_type::int64_type app_id);

        void set_title(traits::param_type::string_type title);

        void set_tag(traits::param_type::int32_type tag);

        void set_banner_url(traits::param_type::string_type banner_url);

        void set_content(traits::param_type::string_type content);

        void set_priority(traits::param_type::int32_type priority);

        void set_publish_time(traits::param_type::int64_type publish_time);

        void set_status(traits::param_type::int32_type status);

        void set_server(traits::param_type::string_type server);

        void set_create_time(traits::param_type::int64_type create_time);

        void set_update_time(traits::param_type::int64_type update_time);

        [[nodiscard]] static const database_field_container& get_database_field_container();

        [[nodiscard]] static const_database_entity_change_shared_ptr get_select(database_type database_type);

        [[nodiscard]] static const_database_entity_change_shared_ptr get_select(database_type database_type, traits::param_type::int64_type id);

        [[nodiscard]] static const_database_entity_change_shared_ptr get_select(database_type database_type, const const_basis_database_container_shared_ptr& key);

        static constexpr std::string_view database_name{ "announcement" };

        static constexpr std::string_view id_describe{ "id" };
        static constexpr std::string_view app_id_describe{ "app_id" };
        static constexpr std::string_view title_describe{ "title" };
        static constexpr std::string_view tag_describe{ "tag" };
        static constexpr std::string_view banner_url_describe{ "banner_url" };
        static constexpr std::string_view content_describe{ "content" };
        static constexpr std::string_view priority_describe{ "priority" };
        static constexpr std::string_view publish_time_describe{ "publish_time" };
        static constexpr std::string_view status_describe{ "status" };
        static constexpr std::string_view server_describe{ "server" };
        static constexpr std::string_view create_time_describe{ "create_time" };
        static constexpr std::string_view update_time_describe{ "update_time" };

    private:
        [[nodiscard]] static const_basis_database_container_shared_ptr get_key_basis_database_container(traits::param_type::int64_type id);

        entity<id_describe, database_data_type::int64_type, database_index_type::key> id_;
        entity<app_id_describe, database_data_type::int64_type, database_index_type::index> app_id_;
        entity<title_describe, database_data_type::string_type> title_;
        entity<tag_describe, database_data_type::int32_type> tag_;
        entity<banner_url_describe, database_data_type::string_type> banner_url_;
        entity<content_describe, database_data_type::string_type> content_;
        entity<priority_describe, database_data_type::int32_type> priority_;
        entity<publish_time_describe, database_data_type::int64_type> publish_time_;
        entity<status_describe, database_data_type::int32_type> status_;
        entity<server_describe, database_data_type::string_type> server_;
        entity<create_time_describe, database_data_type::int64_type> create_time_;
        entity<update_time_describe, database_data_type::int64_type> update_time_;
    };
}