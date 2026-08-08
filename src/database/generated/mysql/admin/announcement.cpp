// 此文件是自动生成，请勿手动修改。

#include "announcement.h"
#include "config/basic/database_type.h"
#include "database/basic/basis_database.tpp"
#include "database/basic/database_change_type.h"
#include "database/basic/database_entity_change.tpp"
#include "database/entity/database_entity.h"
#include "database/entity/entity.tpp"

celeritas::announcement::announcement(const database_entity_change& entity)
    : base_type{ entity.get_database_entity_change(id_describe)  },
      id_{ entity.get_value<database_data_type::int64_type>(id_describe) },
      app_id_{ entity.get_value<database_data_type::int64_type>(app_id_describe) },
      title_{ entity.get_value<database_data_type::string_type>(title_describe) },
      tag_{ entity.get_value<database_data_type::int32_type>(tag_describe) },
      banner_url_{ entity.get_value<database_data_type::string_type>(banner_url_describe) },
      content_{ entity.get_value<database_data_type::string_type>(content_describe) },
      priority_{ entity.get_value<database_data_type::int32_type>(priority_describe) },
      publish_time_{ entity.get_value<database_data_type::int64_type>(publish_time_describe) },
      status_{ entity.get_value<database_data_type::int32_type>(status_describe) },
      server_{ entity.get_value<database_data_type::string_type>(server_describe) },
      create_time_{ entity.get_value<database_data_type::int64_type>(create_time_describe) },
      update_time_{ entity.get_value<database_data_type::int64_type>(update_time_describe) }
{
}

celeritas::announcement::announcement(const database_type database_type, const database_entity_change& entity)
    : base_type{ database_type, entity.get_database_entity_change(id_describe) },
      id_{ entity.get_value<database_data_type::int64_type>(id_describe) },
      app_id_{ entity.get_value<database_data_type::int64_type>(app_id_describe) },
      title_{ entity.get_value<database_data_type::string_type>(title_describe) },
      tag_{ entity.get_value<database_data_type::int32_type>(tag_describe) },
      banner_url_{ entity.get_value<database_data_type::string_type>(banner_url_describe) },
      content_{ entity.get_value<database_data_type::string_type>(content_describe) },
      priority_{ entity.get_value<database_data_type::int32_type>(priority_describe) },
      publish_time_{ entity.get_value<database_data_type::int64_type>(publish_time_describe) },
      status_{ entity.get_value<database_data_type::int32_type>(status_describe) },
      server_{ entity.get_value<database_data_type::string_type>(server_describe) },
      create_time_{ entity.get_value<database_data_type::int64_type>(create_time_describe) },
      update_time_{ entity.get_value<database_data_type::int64_type>(update_time_describe) }
{
    if (database_type != entity.get_database_type())
    {
        add_modify(id_describe, get_id());
        add_modify(app_id_describe, get_app_id());
        add_modify(title_describe, get_title());
        add_modify(tag_describe, get_tag());
        add_modify(banner_url_describe, get_banner_url());
        add_modify(content_describe, get_content());
        add_modify(priority_describe, get_priority());
        add_modify(publish_time_describe, get_publish_time());
        add_modify(status_describe, get_status());
        add_modify(server_describe, get_server());
        add_modify(create_time_describe, get_create_time());
        add_modify(update_time_describe, get_update_time());
    }
}

celeritas::announcement::announcement(const database_type database_type, traits::param_type::int64_type id)
    : base_type{ database_type, database_name, get_key_basis_database_container(id) },
      id_{ id },
      app_id_{ traits::int64_type{} },
      title_{ traits::string_type{} },
      tag_{ traits::int32_type{} },
      banner_url_{ traits::string_type{} },
      content_{ traits::string_type{} },
      priority_{ traits::int32_type{} },
      publish_time_{ traits::int64_type{} },
      status_{ traits::int32_type{} },
      server_{ traits::string_type{} },
      create_time_{ traits::int64_type{} },
      update_time_{ traits::int64_type{} }
{
    add_modify(id_describe, id);
}

celeritas::traits::int64_type celeritas::announcement::get_id() const noexcept
{
    return id_.get_value();
}

celeritas::traits::int64_type celeritas::announcement::get_app_id() const noexcept
{
    return app_id_.get_value();
}

celeritas::traits::string_type celeritas::announcement::get_title() const
{
    return title_.get_value();
}

celeritas::traits::int32_type celeritas::announcement::get_tag() const noexcept
{
    return tag_.get_value();
}

celeritas::traits::string_type celeritas::announcement::get_banner_url() const
{
    return banner_url_.get_value();
}

celeritas::traits::string_type celeritas::announcement::get_content() const
{
    return content_.get_value();
}

celeritas::traits::int32_type celeritas::announcement::get_priority() const noexcept
{
    return priority_.get_value();
}

celeritas::traits::int64_type celeritas::announcement::get_publish_time() const noexcept
{
    return publish_time_.get_value();
}

celeritas::traits::int32_type celeritas::announcement::get_status() const noexcept
{
    return status_.get_value();
}

celeritas::traits::string_type celeritas::announcement::get_server() const
{
    return server_.get_value();
}

celeritas::traits::int64_type celeritas::announcement::get_create_time() const noexcept
{
    return create_time_.get_value();
}

celeritas::traits::int64_type celeritas::announcement::get_update_time() const noexcept
{
    return update_time_.get_value();
}

void celeritas::announcement::set_id(traits::param_type::int64_type id)
{
    if (id != get_id())
    {
        id_.set_value(id);

        add_modify(id_describe, get_id());
    }
}

void celeritas::announcement::set_app_id(traits::param_type::int64_type app_id)
{
    if (app_id != get_app_id())
    {
        app_id_.set_value(app_id);

        add_modify(app_id_describe, get_app_id());
    }
}

void celeritas::announcement::set_title(traits::param_type::string_type title)
{
    if (title != get_title())
    {
        title_.set_value(title);

        add_modify(title_describe, get_title());
    }
}

void celeritas::announcement::set_tag(traits::param_type::int32_type tag)
{
    if (tag != get_tag())
    {
        tag_.set_value(tag);

        add_modify(tag_describe, get_tag());
    }
}

void celeritas::announcement::set_banner_url(traits::param_type::string_type banner_url)
{
    if (banner_url != get_banner_url())
    {
        banner_url_.set_value(banner_url);

        add_modify(banner_url_describe, get_banner_url());
    }
}

void celeritas::announcement::set_content(traits::param_type::string_type content)
{
    if (content != get_content())
    {
        content_.set_value(content);

        add_modify(content_describe, get_content());
    }
}

void celeritas::announcement::set_priority(traits::param_type::int32_type priority)
{
    if (priority != get_priority())
    {
        priority_.set_value(priority);

        add_modify(priority_describe, get_priority());
    }
}

void celeritas::announcement::set_publish_time(traits::param_type::int64_type publish_time)
{
    if (publish_time != get_publish_time())
    {
        publish_time_.set_value(publish_time);

        add_modify(publish_time_describe, get_publish_time());
    }
}

void celeritas::announcement::set_status(traits::param_type::int32_type status)
{
    if (status != get_status())
    {
        status_.set_value(status);

        add_modify(status_describe, get_status());
    }
}

void celeritas::announcement::set_server(traits::param_type::string_type server)
{
    if (server != get_server())
    {
        server_.set_value(server);

        add_modify(server_describe, get_server());
    }
}

void celeritas::announcement::set_create_time(traits::param_type::int64_type create_time)
{
    if (create_time != get_create_time())
    {
        create_time_.set_value(create_time);

        add_modify(create_time_describe, get_create_time());
    }
}

void celeritas::announcement::set_update_time(traits::param_type::int64_type update_time)
{
    if (update_time != get_update_time())
    {
        update_time_.set_value(update_time);

        add_modify(update_time_describe, get_update_time());
    }
}

const celeritas::database_entity::database_field_container& celeritas::announcement::get_database_field_container()
{
    static const database_field_container field_name_container{ decltype(id_)::get_database_field(),
                                                                decltype(app_id_)::get_database_field(),
                                                                decltype(title_)::get_database_field(),
                                                                decltype(tag_)::get_database_field(),
                                                                decltype(banner_url_)::get_database_field(),
                                                                decltype(content_)::get_database_field(),
                                                                decltype(priority_)::get_database_field(),
                                                                decltype(publish_time_)::get_database_field(),
                                                                decltype(status_)::get_database_field(),
                                                                decltype(server_)::get_database_field(),
                                                                decltype(create_time_)::get_database_field(),
                                                                decltype(update_time_)::get_database_field() };

    return field_name_container;
}

celeritas::announcement::const_database_entity_change_shared_ptr celeritas::announcement::get_select(const database_type database_type)
{
    static const auto result = std::make_shared<database_entity_change>(database_type,
                                                                        database_name,
                                                                        database_change_type::select_type);

    return result;
}

celeritas::announcement::const_database_entity_change_shared_ptr celeritas::announcement::get_select(const database_type database_type, traits::param_type::int64_type id)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    get_key_basis_database_container(id));
}

celeritas::announcement::const_database_entity_change_shared_ptr celeritas::announcement::get_select(const database_type database_type, const const_basis_database_container_shared_ptr& key)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    key);
}

celeritas::announcement::const_basis_database_container_shared_ptr celeritas::announcement::get_key_basis_database_container(traits::param_type::int64_type id)
{
    return std::make_shared<basis_database_container>(basis_database{ id_describe, id });
}