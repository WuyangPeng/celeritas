// 创建时间：2026-08-05
// 修改时间：2026-08-05
// 审核时间：2026-08-05

#include "feedback_admin_list_item.h"

celeritas::feedback_admin_list_item::feedback_admin_list_item(const int64_t id,
                                                              const int64_t app_id,
                                                              const int32_t type,
                                                              std::string content,
                                                              std::string device_info,
                                                              std::string server,
                                                              std::string player_id,
                                                              const bool is_anonymous,
                                                              std::string image_data,
                                                              const int64_t create_time)
    : id_{ id },
      app_id_{ app_id },
      type_{ type },
      content_{ std::move(content) },
      device_info_{ std::move(device_info) },
      server_{ std::move(server) },
      player_id_{ std::move(player_id) },
      is_anonymous_{ is_anonymous },
      image_data_{ std::move(image_data) },
      create_time_{ create_time }
{
}

int64_t celeritas::feedback_admin_list_item::get_id() const noexcept
{
    return id_;
}

int64_t celeritas::feedback_admin_list_item::get_app_id() const noexcept
{
    return app_id_;
}

int32_t celeritas::feedback_admin_list_item::get_type() const noexcept
{
    return type_;
}

const std::string& celeritas::feedback_admin_list_item::get_content() const noexcept
{
    return content_;
}

const std::string& celeritas::feedback_admin_list_item::get_device_info() const noexcept
{
    return device_info_;
}

const std::string& celeritas::feedback_admin_list_item::get_server() const noexcept
{
    return server_;
}

const std::string& celeritas::feedback_admin_list_item::get_player_id() const noexcept
{
    return player_id_;
}

bool celeritas::feedback_admin_list_item::get_is_anonymous() const noexcept
{
    return is_anonymous_;
}

const std::string& celeritas::feedback_admin_list_item::get_image_data() const noexcept
{
    return image_data_;
}

int64_t celeritas::feedback_admin_list_item::get_create_time() const noexcept
{
    return create_time_;
}

celeritas::feedback_admin_list_item celeritas::tag_invoke(feedback_admin_list_item_tag, const feedback_admin_list_item::json_value& value)
{
    const auto& object = value.as_object();

    return feedback_admin_list_item{ boost::json::value_to<int64_t>(object.at(feedback_admin_list_item::id_description)),
                                     boost::json::value_to<int64_t>(object.at(feedback_admin_list_item::app_id_description)),
                                     boost::json::value_to<int32_t>(object.at(feedback_admin_list_item::type_description)),
                                     boost::json::value_to<std::string>(object.at(feedback_admin_list_item::content_description)),
                                     boost::json::value_to<std::string>(object.at(feedback_admin_list_item::device_info_description)),
                                     boost::json::value_to<std::string>(object.at(feedback_admin_list_item::server_description)),
                                     boost::json::value_to<std::string>(object.at(feedback_admin_list_item::player_id_description)),
                                     boost::json::value_to<bool>(object.at(feedback_admin_list_item::is_anonymous_description)),
                                     boost::json::value_to<std::string>(object.at(feedback_admin_list_item::image_data_description)),
                                     boost::json::value_to<int64_t>(object.at(feedback_admin_list_item::create_time_description)) };
}

void celeritas::tag_invoke(boost::json::value_from_tag, feedback_admin_list_item::json_value& value, const feedback_admin_list_item& item)
{
    value = {
        { feedback_admin_list_item::id_description, item.get_id() },
        { feedback_admin_list_item::app_id_description, item.get_app_id() },
        { feedback_admin_list_item::type_description, item.get_type() },
        { feedback_admin_list_item::content_description, item.get_content() },
        { feedback_admin_list_item::device_info_description, item.get_device_info() },
        { feedback_admin_list_item::server_description, item.get_server() },
        { feedback_admin_list_item::player_id_description, item.get_player_id() },
        { feedback_admin_list_item::is_anonymous_description, item.get_is_anonymous() },
        { feedback_admin_list_item::image_data_description, item.get_image_data() },
        { feedback_admin_list_item::create_time_description, item.get_create_time() }
    };
}
