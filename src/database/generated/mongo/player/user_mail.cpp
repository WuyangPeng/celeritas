// 此文件是自动生成，请勿手动修改。

#include "user_mail.h"
#include "config/basic/database_type.h"
#include "database/basic/basis_database.tpp"
#include "database/basic/database_change_type.h"
#include "database/basic/database_entity_change.tpp"
#include "database/entity/database_entity.h"
#include "database/entity/entity.tpp"

celeritas::user_mail::user_mail(const database_entity_change& entity)
    : base_type{ entity.get_database_entity_change(id_describe)  },
      id_{ entity.get_value<database_data_type::int64_type>(id_describe) },
      user_id_{ entity.get_value<database_data_type::int64_type>(user_id_describe) },
      server_mail_id_{ entity.get_value<database_data_type::int64_type>(server_mail_id_describe) },
      type_{ entity.get_value<database_data_type::int32_type>(type_describe) },
      multilingual_{ entity.get_value<database_data_type::bool_type>(multilingual_describe) },
      title_{ entity.get_value<database_data_type::document_type>(title_describe) },
      content_{ entity.get_value<database_data_type::document_type>(content_describe) },
      attachments_{ entity.get_value<database_data_type::document_array_type>(attachments_describe) },
      status_{ entity.get_value<database_data_type::int32_type>(status_describe) },
      send_time_{ entity.get_value<database_data_type::int64_type>(send_time_describe) },
      expire_time_{ entity.get_value<database_data_type::int64_type>(expire_time_describe) }
{
}

celeritas::user_mail::user_mail(const database_type database_type, const database_entity_change& entity)
    : base_type{ database_type, entity.get_database_entity_change(id_describe) },
      id_{ entity.get_value<database_data_type::int64_type>(id_describe) },
      user_id_{ entity.get_value<database_data_type::int64_type>(user_id_describe) },
      server_mail_id_{ entity.get_value<database_data_type::int64_type>(server_mail_id_describe) },
      type_{ entity.get_value<database_data_type::int32_type>(type_describe) },
      multilingual_{ entity.get_value<database_data_type::bool_type>(multilingual_describe) },
      title_{ entity.get_value<database_data_type::document_type>(title_describe) },
      content_{ entity.get_value<database_data_type::document_type>(content_describe) },
      attachments_{ entity.get_value<database_data_type::document_array_type>(attachments_describe) },
      status_{ entity.get_value<database_data_type::int32_type>(status_describe) },
      send_time_{ entity.get_value<database_data_type::int64_type>(send_time_describe) },
      expire_time_{ entity.get_value<database_data_type::int64_type>(expire_time_describe) }
{
    if (database_type != entity.get_database_type())
    {
        add_modify(id_describe, get_id());
        add_modify(user_id_describe, get_user_id());
        add_modify(server_mail_id_describe, get_server_mail_id());
        add_modify(type_describe, get_type());
        add_modify(multilingual_describe, is_multilingual());
        add_modify(title_describe, get_title());
        add_modify(content_describe, get_content());
        add_modify(attachments_describe, get_attachments());
        add_modify(status_describe, get_status());
        add_modify(send_time_describe, get_send_time());
        add_modify(expire_time_describe, get_expire_time());
    }
}

celeritas::user_mail::user_mail(const database_type database_type, traits::param_type::int64_type id)
    : base_type{ database_type, database_name, get_key_basis_database_container(id) },
      id_{ id },
      user_id_{ traits::int64_type{} },
      server_mail_id_{ traits::int64_type{} },
      type_{ traits::int32_type{} },
      multilingual_{ traits::bool_type{} },
      title_{ traits::document_type{} },
      content_{ traits::document_type{} },
      attachments_{ traits::document_array_type{} },
      status_{ traits::int32_type{} },
      send_time_{ traits::int64_type{} },
      expire_time_{ traits::int64_type{} }
{
    add_modify(id_describe, id);
}

celeritas::traits::int64_type celeritas::user_mail::get_id() const noexcept
{
    return id_.get_value();
}

celeritas::traits::int64_type celeritas::user_mail::get_user_id() const noexcept
{
    return user_id_.get_value();
}

celeritas::traits::int64_type celeritas::user_mail::get_server_mail_id() const noexcept
{
    return server_mail_id_.get_value();
}

celeritas::traits::int32_type celeritas::user_mail::get_type() const noexcept
{
    return type_.get_value();
}

celeritas::traits::bool_type celeritas::user_mail::is_multilingual() const noexcept
{
    return multilingual_.get_value();
}

celeritas::traits::document_type celeritas::user_mail::get_title() const
{
    return title_.get_value();
}

celeritas::traits::document_type celeritas::user_mail::get_content() const
{
    return content_.get_value();
}

celeritas::traits::document_array_type celeritas::user_mail::get_attachments() const
{
    return attachments_.get_value();
}

celeritas::traits::int32_type celeritas::user_mail::get_status() const noexcept
{
    return status_.get_value();
}

celeritas::traits::int64_type celeritas::user_mail::get_send_time() const noexcept
{
    return send_time_.get_value();
}

celeritas::traits::int64_type celeritas::user_mail::get_expire_time() const noexcept
{
    return expire_time_.get_value();
}

void celeritas::user_mail::set_id(traits::param_type::int64_type id)
{
    if (id != get_id())
    {
        id_.set_value(id);

        add_modify(id_describe, get_id());
    }
}

void celeritas::user_mail::set_user_id(traits::param_type::int64_type user_id)
{
    if (user_id != get_user_id())
    {
        user_id_.set_value(user_id);

        add_modify(user_id_describe, get_user_id());
    }
}

void celeritas::user_mail::set_server_mail_id(traits::param_type::int64_type server_mail_id)
{
    if (server_mail_id != get_server_mail_id())
    {
        server_mail_id_.set_value(server_mail_id);

        add_modify(server_mail_id_describe, get_server_mail_id());
    }
}

void celeritas::user_mail::set_type(traits::param_type::int32_type type)
{
    if (type != get_type())
    {
        type_.set_value(type);

        add_modify(type_describe, get_type());
    }
}

void celeritas::user_mail::set_multilingual(traits::param_type::bool_type multilingual)
{
    if (multilingual != is_multilingual())
    {
        multilingual_.set_value(multilingual);

        add_modify(multilingual_describe, is_multilingual());
    }
}

void celeritas::user_mail::set_title(traits::param_type::document_type title)
{
    if (title != get_title())
    {
        title_.set_value(title);

        add_modify(title_describe, get_title());
    }
}

void celeritas::user_mail::set_content(traits::param_type::document_type content)
{
    if (content != get_content())
    {
        content_.set_value(content);

        add_modify(content_describe, get_content());
    }
}

void celeritas::user_mail::set_attachments(traits::param_type::document_array_type attachments)
{
    if (attachments != get_attachments())
    {
        attachments_.set_value(attachments);

        add_modify(attachments_describe, get_attachments());
    }
}

void celeritas::user_mail::set_status(traits::param_type::int32_type status)
{
    if (status != get_status())
    {
        status_.set_value(status);

        add_modify(status_describe, get_status());
    }
}

void celeritas::user_mail::set_send_time(traits::param_type::int64_type send_time)
{
    if (send_time != get_send_time())
    {
        send_time_.set_value(send_time);

        add_modify(send_time_describe, get_send_time());
    }
}

void celeritas::user_mail::set_expire_time(traits::param_type::int64_type expire_time)
{
    if (expire_time != get_expire_time())
    {
        expire_time_.set_value(expire_time);

        add_modify(expire_time_describe, get_expire_time());
    }
}

void celeritas::user_mail::set_attachments(const int index, traits::param_type::document_array_element_type attachments)
{
    if (attachments_.set_value(index, attachments))
    {
        add_modify(attachments_describe, get_attachments());
    }
}

void celeritas::user_mail::add_attachments(traits::param_type::document_array_element_type attachments)
{
    attachments_.add_value(attachments);

    add_modify(attachments_describe, get_attachments());
}

void celeritas::user_mail::remove_attachments(const int index)
{
    attachments_.remove_value(index);

    add_modify(attachments_describe, get_attachments());
}

const celeritas::database_entity::database_field_container& celeritas::user_mail::get_database_field_container()
{
    static const database_field_container field_name_container{ decltype(id_)::get_database_field(),
                                                                decltype(user_id_)::get_database_field(),
                                                                decltype(server_mail_id_)::get_database_field(),
                                                                decltype(type_)::get_database_field(),
                                                                decltype(multilingual_)::get_database_field(),
                                                                decltype(title_)::get_database_field(),
                                                                decltype(content_)::get_database_field(),
                                                                decltype(attachments_)::get_database_field(),
                                                                decltype(status_)::get_database_field(),
                                                                decltype(send_time_)::get_database_field(),
                                                                decltype(expire_time_)::get_database_field() };

    return field_name_container;
}

celeritas::user_mail::const_database_entity_change_shared_ptr celeritas::user_mail::get_select(const database_type database_type)
{
    static const auto result = std::make_shared<database_entity_change>(database_type,
                                                                        database_name,
                                                                        database_change_type::select_type);

    return result;
}

celeritas::user_mail::const_database_entity_change_shared_ptr celeritas::user_mail::get_select(const database_type database_type, traits::param_type::int64_type id)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    get_key_basis_database_container(id));
}

celeritas::user_mail::const_database_entity_change_shared_ptr celeritas::user_mail::get_select(const database_type database_type, const const_basis_database_container_shared_ptr& key)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    key);
}

celeritas::user_mail::const_basis_database_container_shared_ptr celeritas::user_mail::get_key_basis_database_container(traits::param_type::int64_type id)
{
    return std::make_shared<basis_database_container>(basis_database{ id_describe, id });
}