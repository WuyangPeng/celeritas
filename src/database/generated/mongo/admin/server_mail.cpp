// 此文件是自动生成，请勿手动修改。

#include "server_mail.h"
#include "config/basic/database_type.h"
#include "database/basic/basis_database.tpp"
#include "database/basic/database_change_type.h"
#include "database/basic/database_entity_change.tpp"
#include "database/entity/database_entity.h"
#include "database/entity/entity.tpp"

celeritas::server_mail::server_mail(const database_entity_change& entity)
    : base_type{ entity.get_database_entity_change(id_describe)  },
      id_{ entity.get_value<database_data_type::int64_type>(id_describe) },
      type_{ entity.get_value<database_data_type::int32_type>(type_describe) },
      title_{ entity.get_value<database_data_type::document_type>(title_describe) },
      content_{ entity.get_value<database_data_type::document_type>(content_describe) },
      attachments_{ entity.get_value<database_data_type::document_array_type>(attachments_describe) },
      send_time_{ entity.get_value<database_data_type::int64_type>(send_time_describe) },
      expire_time_{ entity.get_value<database_data_type::int64_type>(expire_time_describe) }
{
}

celeritas::server_mail::server_mail(const database_type database_type, const database_entity_change& entity)
    : base_type{ database_type, entity.get_database_entity_change(id_describe) },
      id_{ entity.get_value<database_data_type::int64_type>(id_describe) },
      type_{ entity.get_value<database_data_type::int32_type>(type_describe) },
      title_{ entity.get_value<database_data_type::document_type>(title_describe) },
      content_{ entity.get_value<database_data_type::document_type>(content_describe) },
      attachments_{ entity.get_value<database_data_type::document_array_type>(attachments_describe) },
      send_time_{ entity.get_value<database_data_type::int64_type>(send_time_describe) },
      expire_time_{ entity.get_value<database_data_type::int64_type>(expire_time_describe) }
{
    if (database_type != entity.get_database_type())
    {
        add_modify(id_describe, get_id());
        add_modify(type_describe, get_type());
        add_modify(title_describe, get_title());
        add_modify(content_describe, get_content());
        add_modify(attachments_describe, get_attachments());
        add_modify(send_time_describe, get_send_time());
        add_modify(expire_time_describe, get_expire_time());
    }
}

celeritas::server_mail::server_mail(const database_type database_type, traits::param_type::int64_type id)
    : base_type{ database_type, database_name, get_key_basis_database_container(id) },
      id_{ id },
      type_{ traits::int32_type{} },
      title_{ traits::document_type{} },
      content_{ traits::document_type{} },
      attachments_{ traits::document_array_type{} },
      send_time_{ traits::int64_type{} },
      expire_time_{ traits::int64_type{} }
{
    add_modify(id_describe, id);
}

celeritas::traits::int64_type celeritas::server_mail::get_id() const noexcept
{
    return id_.get_value();
}

celeritas::traits::int32_type celeritas::server_mail::get_type() const noexcept
{
    return type_.get_value();
}

celeritas::traits::document_type celeritas::server_mail::get_title() const
{
    return title_.get_value();
}

celeritas::traits::document_type celeritas::server_mail::get_content() const
{
    return content_.get_value();
}

celeritas::traits::document_array_type celeritas::server_mail::get_attachments() const
{
    return attachments_.get_value();
}

celeritas::traits::int64_type celeritas::server_mail::get_send_time() const noexcept
{
    return send_time_.get_value();
}

celeritas::traits::int64_type celeritas::server_mail::get_expire_time() const noexcept
{
    return expire_time_.get_value();
}

void celeritas::server_mail::set_id(traits::param_type::int64_type id)
{
    if (id != get_id())
    {
        id_.set_value(id);

        add_modify(id_describe, get_id());
    }
}

void celeritas::server_mail::set_type(traits::param_type::int32_type type)
{
    if (type != get_type())
    {
        type_.set_value(type);

        add_modify(type_describe, get_type());
    }
}

void celeritas::server_mail::set_title(traits::param_type::document_type title)
{
    if (title != get_title())
    {
        title_.set_value(title);

        add_modify(title_describe, get_title());
    }
}

void celeritas::server_mail::set_content(traits::param_type::document_type content)
{
    if (content != get_content())
    {
        content_.set_value(content);

        add_modify(content_describe, get_content());
    }
}

void celeritas::server_mail::set_attachments(traits::param_type::document_array_type attachments)
{
    if (attachments != get_attachments())
    {
        attachments_.set_value(attachments);

        add_modify(attachments_describe, get_attachments());
    }
}

void celeritas::server_mail::set_send_time(traits::param_type::int64_type send_time)
{
    if (send_time != get_send_time())
    {
        send_time_.set_value(send_time);

        add_modify(send_time_describe, get_send_time());
    }
}

void celeritas::server_mail::set_expire_time(traits::param_type::int64_type expire_time)
{
    if (expire_time != get_expire_time())
    {
        expire_time_.set_value(expire_time);

        add_modify(expire_time_describe, get_expire_time());
    }
}

void celeritas::server_mail::set_attachments(const int index, traits::param_type::document_array_element_type attachments)
{
    if (attachments_.set_value(index, attachments))
    {
        add_modify(attachments_describe, get_attachments());
    }
}

void celeritas::server_mail::add_attachments(traits::param_type::document_array_element_type attachments)
{
    attachments_.add_value(attachments);

    add_modify(attachments_describe, get_attachments());
}

void celeritas::server_mail::remove_attachments(const int index)
{
    attachments_.remove_value(index);

    add_modify(attachments_describe, get_attachments());
}

const celeritas::database_entity::database_field_container& celeritas::server_mail::get_database_field_container()
{
    static const database_field_container field_name_container{ decltype(id_)::get_database_field(),
                                                                decltype(type_)::get_database_field(),
                                                                decltype(title_)::get_database_field(),
                                                                decltype(content_)::get_database_field(),
                                                                decltype(attachments_)::get_database_field(),
                                                                decltype(send_time_)::get_database_field(),
                                                                decltype(expire_time_)::get_database_field() };

    return field_name_container;
}

celeritas::server_mail::const_database_entity_change_shared_ptr celeritas::server_mail::get_select(const database_type database_type)
{
    static const auto result = std::make_shared<database_entity_change>(database_type,
                                                                        database_name,
                                                                        database_change_type::select_type);

    return result;
}

celeritas::server_mail::const_database_entity_change_shared_ptr celeritas::server_mail::get_select(const database_type database_type, traits::param_type::int64_type id)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    get_key_basis_database_container(id));
}

celeritas::server_mail::const_database_entity_change_shared_ptr celeritas::server_mail::get_select(const database_type database_type, const const_basis_database_container_shared_ptr& key)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    key);
}

celeritas::server_mail::const_basis_database_container_shared_ptr celeritas::server_mail::get_key_basis_database_container(traits::param_type::int64_type id)
{
    return std::make_shared<basis_database_container>(basis_database{ id_describe, id });
}