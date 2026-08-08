// 此文件是自动生成，请勿手动修改。

#include "feedback.h"
#include "config/basic/database_type.h"
#include "database/basic/basis_database.tpp"
#include "database/basic/database_change_type.h"
#include "database/basic/database_entity_change.tpp"
#include "database/entity/database_entity.h"
#include "database/entity/entity.tpp"

celeritas::feedback::feedback(const database_entity_change& entity)
    : base_type{ entity.get_database_entity_change(id_describe)  },
      id_{ entity.get_value<database_data_type::int64_type>(id_describe) },
      app_id_{ entity.get_value<database_data_type::int64_type>(app_id_describe) },
      type_{ entity.get_value<database_data_type::int32_type>(type_describe) },
      content_{ entity.get_value<database_data_type::string_type>(content_describe) },
      device_info_{ entity.get_value<database_data_type::string_type>(device_info_describe) },
      server_{ entity.get_value<database_data_type::string_type>(server_describe) },
      player_id_{ entity.get_value<database_data_type::string_type>(player_id_describe) },
      is_anonymous_{ entity.get_value<database_data_type::bool_type>(is_anonymous_describe) },
      image_data_{ entity.get_value<database_data_type::byte_array_type>(image_data_describe) },
      create_time_{ entity.get_value<database_data_type::int64_type>(create_time_describe) }
{
}

celeritas::feedback::feedback(const database_type database_type, const database_entity_change& entity)
    : base_type{ database_type, entity.get_database_entity_change(id_describe) },
      id_{ entity.get_value<database_data_type::int64_type>(id_describe) },
      app_id_{ entity.get_value<database_data_type::int64_type>(app_id_describe) },
      type_{ entity.get_value<database_data_type::int32_type>(type_describe) },
      content_{ entity.get_value<database_data_type::string_type>(content_describe) },
      device_info_{ entity.get_value<database_data_type::string_type>(device_info_describe) },
      server_{ entity.get_value<database_data_type::string_type>(server_describe) },
      player_id_{ entity.get_value<database_data_type::string_type>(player_id_describe) },
      is_anonymous_{ entity.get_value<database_data_type::bool_type>(is_anonymous_describe) },
      image_data_{ entity.get_value<database_data_type::byte_array_type>(image_data_describe) },
      create_time_{ entity.get_value<database_data_type::int64_type>(create_time_describe) }
{
    if (database_type != entity.get_database_type())
    {
        add_modify(id_describe, get_id());
        add_modify(app_id_describe, get_app_id());
        add_modify(type_describe, get_type());
        add_modify(content_describe, get_content());
        add_modify(device_info_describe, get_device_info());
        add_modify(server_describe, get_server());
        add_modify(player_id_describe, get_player_id());
        add_modify(is_anonymous_describe, is_is_anonymous());
        add_modify(image_data_describe, get_image_data());
        add_modify(create_time_describe, get_create_time());
    }
}

celeritas::feedback::feedback(const database_type database_type, traits::param_type::int64_type id)
    : base_type{ database_type, database_name, get_key_basis_database_container(id) },
      id_{ id },
      app_id_{ traits::int64_type{} },
      type_{ traits::int32_type{} },
      content_{ traits::string_type{} },
      device_info_{ traits::string_type{} },
      server_{ traits::string_type{} },
      player_id_{ traits::string_type{} },
      is_anonymous_{ traits::bool_type{} },
      image_data_{ traits::byte_array_type{} },
      create_time_{ traits::int64_type{} }
{
    add_modify(id_describe, id);
}

celeritas::traits::int64_type celeritas::feedback::get_id() const noexcept
{
    return id_.get_value();
}

celeritas::traits::int64_type celeritas::feedback::get_app_id() const noexcept
{
    return app_id_.get_value();
}

celeritas::traits::int32_type celeritas::feedback::get_type() const noexcept
{
    return type_.get_value();
}

celeritas::traits::string_type celeritas::feedback::get_content() const
{
    return content_.get_value();
}

celeritas::traits::string_type celeritas::feedback::get_device_info() const
{
    return device_info_.get_value();
}

celeritas::traits::string_type celeritas::feedback::get_server() const
{
    return server_.get_value();
}

celeritas::traits::string_type celeritas::feedback::get_player_id() const
{
    return player_id_.get_value();
}

celeritas::traits::bool_type celeritas::feedback::is_is_anonymous() const noexcept
{
    return is_anonymous_.get_value();
}

celeritas::traits::byte_array_type celeritas::feedback::get_image_data() const
{
    return image_data_.get_value();
}

celeritas::traits::int64_type celeritas::feedback::get_create_time() const noexcept
{
    return create_time_.get_value();
}

void celeritas::feedback::set_id(traits::param_type::int64_type id)
{
    if (id != get_id())
    {
        id_.set_value(id);

        add_modify(id_describe, get_id());
    }
}

void celeritas::feedback::set_app_id(traits::param_type::int64_type app_id)
{
    if (app_id != get_app_id())
    {
        app_id_.set_value(app_id);

        add_modify(app_id_describe, get_app_id());
    }
}

void celeritas::feedback::set_type(traits::param_type::int32_type type)
{
    if (type != get_type())
    {
        type_.set_value(type);

        add_modify(type_describe, get_type());
    }
}

void celeritas::feedback::set_content(traits::param_type::string_type content)
{
    if (content != get_content())
    {
        content_.set_value(content);

        add_modify(content_describe, get_content());
    }
}

void celeritas::feedback::set_device_info(traits::param_type::string_type device_info)
{
    if (device_info != get_device_info())
    {
        device_info_.set_value(device_info);

        add_modify(device_info_describe, get_device_info());
    }
}

void celeritas::feedback::set_server(traits::param_type::string_type server)
{
    if (server != get_server())
    {
        server_.set_value(server);

        add_modify(server_describe, get_server());
    }
}

void celeritas::feedback::set_player_id(traits::param_type::string_type player_id)
{
    if (player_id != get_player_id())
    {
        player_id_.set_value(player_id);

        add_modify(player_id_describe, get_player_id());
    }
}

void celeritas::feedback::set_is_anonymous(traits::param_type::bool_type is_anonymous)
{
    if (is_anonymous != is_is_anonymous())
    {
        is_anonymous_.set_value(is_anonymous);

        add_modify(is_anonymous_describe, is_is_anonymous());
    }
}

void celeritas::feedback::set_image_data(traits::param_type::byte_array_type image_data)
{
    if (image_data != get_image_data())
    {
        image_data_.set_value(image_data);

        add_modify(image_data_describe, get_image_data());
    }
}

void celeritas::feedback::set_create_time(traits::param_type::int64_type create_time)
{
    if (create_time != get_create_time())
    {
        create_time_.set_value(create_time);

        add_modify(create_time_describe, get_create_time());
    }
}

void celeritas::feedback::set_image_data(const int index, traits::param_type::byte_array_element_type image_data)
{
    if (image_data_.set_value(index, image_data))
    {
        add_modify(image_data_describe, get_image_data());
    }
}

void celeritas::feedback::add_image_data(traits::param_type::byte_array_element_type image_data)
{
    image_data_.add_value(image_data);

    add_modify(image_data_describe, get_image_data());
}

void celeritas::feedback::remove_image_data(const int index)
{
    image_data_.remove_value(index);

    add_modify(image_data_describe, get_image_data());
}

const celeritas::database_entity::database_field_container& celeritas::feedback::get_database_field_container()
{
    static const database_field_container field_name_container{ decltype(id_)::get_database_field(),
                                                                decltype(app_id_)::get_database_field(),
                                                                decltype(type_)::get_database_field(),
                                                                decltype(content_)::get_database_field(),
                                                                decltype(device_info_)::get_database_field(),
                                                                decltype(server_)::get_database_field(),
                                                                decltype(player_id_)::get_database_field(),
                                                                decltype(is_anonymous_)::get_database_field(),
                                                                decltype(image_data_)::get_database_field(),
                                                                decltype(create_time_)::get_database_field() };

    return field_name_container;
}

celeritas::feedback::const_database_entity_change_shared_ptr celeritas::feedback::get_select(const database_type database_type)
{
    static const auto result = std::make_shared<database_entity_change>(database_type,
                                                                        database_name,
                                                                        database_change_type::select_type);

    return result;
}

celeritas::feedback::const_database_entity_change_shared_ptr celeritas::feedback::get_select(const database_type database_type, traits::param_type::int64_type id)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    get_key_basis_database_container(id));
}

celeritas::feedback::const_database_entity_change_shared_ptr celeritas::feedback::get_select(const database_type database_type, const const_basis_database_container_shared_ptr& key)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    key);
}

celeritas::feedback::const_basis_database_container_shared_ptr celeritas::feedback::get_key_basis_database_container(traits::param_type::int64_type id)
{
    return std::make_shared<basis_database_container>(basis_database{ id_describe, id });
}