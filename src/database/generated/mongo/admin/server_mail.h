// 此文件是自动生成，请勿手动修改。

#pragma once

#include "database/basic/database_data_type_traits.h"
#include "database/basic/database_index_type.h"
#include "database/entity/database_entity.h"
#include "database/entity/entity.h"

namespace celeritas
{
    class server_mail final : public database_entity
    {
    public:
        using class_type = server_mail;
        using base_type = database_entity;

        explicit server_mail(const database_entity_change& entity);

        server_mail(database_type database_type, const database_entity_change& entity);

        server_mail(database_type database_type, traits::param_type::int64_type id);

        [[nodiscard]] traits::int64_type get_id() const noexcept;

        [[nodiscard]] traits::int32_type get_type() const noexcept;

        [[nodiscard]] traits::document_type get_title() const;

        [[nodiscard]] traits::document_type get_content() const;

        [[nodiscard]] traits::document_array_type get_attachments() const;

        [[nodiscard]] traits::int64_type get_send_time() const noexcept;

        [[nodiscard]] traits::int64_type get_expire_time() const noexcept;

        void set_id(traits::param_type::int64_type id);

        void set_type(traits::param_type::int32_type type);

        void set_title(traits::param_type::document_type title);

        void set_content(traits::param_type::document_type content);

        void set_attachments(traits::param_type::document_array_type attachments);

        void set_send_time(traits::param_type::int64_type send_time);

        void set_expire_time(traits::param_type::int64_type expire_time);

        void set_attachments(int index, traits::param_type::document_array_element_type attachments);

        void add_attachments(traits::param_type::document_array_element_type attachments);

        void remove_attachments(int index);

        [[nodiscard]] static const database_field_container& get_database_field_container();

        [[nodiscard]] static const_database_entity_change_shared_ptr get_select(database_type database_type);

        [[nodiscard]] static const_database_entity_change_shared_ptr get_select(database_type database_type, traits::param_type::int64_type id);

        [[nodiscard]] static const_database_entity_change_shared_ptr get_select(database_type database_type, const const_basis_database_container_shared_ptr& key);

        static constexpr std::string_view database_name{ "server_mail" };

        static constexpr std::string_view id_describe{ "_id" };
        static constexpr std::string_view type_describe{ "type" };
        static constexpr std::string_view title_describe{ "title" };
        static constexpr std::string_view content_describe{ "content" };
        static constexpr std::string_view attachments_describe{ "attachments" };
        static constexpr std::string_view send_time_describe{ "send_time" };
        static constexpr std::string_view expire_time_describe{ "expire_time" };

    private:
        [[nodiscard]] static const_basis_database_container_shared_ptr get_key_basis_database_container(traits::param_type::int64_type id);

        entity<id_describe, database_data_type::int64_type, database_index_type::key> id_;
        entity<type_describe, database_data_type::int32_type> type_;
        entity<title_describe, database_data_type::document_type> title_;
        entity<content_describe, database_data_type::document_type> content_;
        entity<attachments_describe, database_data_type::document_array_type> attachments_;
        entity<send_time_describe, database_data_type::int64_type> send_time_;
        entity<expire_time_describe, database_data_type::int64_type> expire_time_;
    };
}