// 创建时间：2026-08-06
// 修改时间：2026-08-06
// 审核时间：2026-08-08

#include "announcement_admin_list_item.h"
#include "common/core/enum_cast.h"

celeritas::announcement_admin_list_item::announcement_admin_list_item(const int64_t id,
                                                                      const int64_t app_id,
                                                                      std::string title,
                                                                      const int32_t tag,
                                                                      std::string banner_url,
                                                                      std::string content,
                                                                      const int32_t priority,
                                                                      const int64_t publish_time,
                                                                      const announcement_status status,
                                                                      std::string server,
                                                                      const int64_t create_time,
                                                                      const int64_t update_time)
    : id_{ id },
      app_id_{ app_id },
      title_{ std::move(title) },
      tag_{ tag },
      banner_url_{ std::move(banner_url) },
      content_{ std::move(content) },
      priority_{ priority },
      publish_time_{ publish_time },
      status_{ status },
      server_{ std::move(server) },
      create_time_{ create_time },
      update_time_{ update_time }
{
}

int64_t celeritas::announcement_admin_list_item::get_id() const noexcept
{
    return id_;
}

int64_t celeritas::announcement_admin_list_item::get_app_id() const noexcept
{
    return app_id_;
}

const std::string& celeritas::announcement_admin_list_item::get_title() const noexcept
{
    return title_;
}

int32_t celeritas::announcement_admin_list_item::get_tag() const noexcept
{
    return tag_;
}

const std::string& celeritas::announcement_admin_list_item::get_banner_url() const noexcept
{
    return banner_url_;
}

const std::string& celeritas::announcement_admin_list_item::get_content() const noexcept
{
    return content_;
}

int32_t celeritas::announcement_admin_list_item::get_priority() const noexcept
{
    return priority_;
}

int64_t celeritas::announcement_admin_list_item::get_publish_time() const noexcept
{
    return publish_time_;
}

celeritas::announcement_status celeritas::announcement_admin_list_item::get_status() const noexcept
{
    return status_;
}

const std::string& celeritas::announcement_admin_list_item::get_server() const noexcept
{
    return server_;
}

int64_t celeritas::announcement_admin_list_item::get_create_time() const noexcept
{
    return create_time_;
}

int64_t celeritas::announcement_admin_list_item::get_update_time() const noexcept
{
    return update_time_;
}

celeritas::announcement_admin_list_item celeritas::tag_invoke(announcement_admin_list_item_tag, const announcement_admin_list_item::json_value& value)
{
    const auto& object = value.as_object();
    const auto id = std::stoll(boost::json::value_to<std::string>(object.at(announcement_admin_list_item::id_description)));

    return announcement_admin_list_item{ id,
                                         boost::json::value_to<int64_t>(object.at(announcement_admin_list_item::app_id_description)),
                                         boost::json::value_to<std::string>(object.at(announcement_admin_list_item::title_description)),
                                         boost::json::value_to<int32_t>(object.at(announcement_admin_list_item::tag_description)),
                                         boost::json::value_to<std::string>(object.at(announcement_admin_list_item::banner_url_description)),
                                         boost::json::value_to<std::string>(object.at(announcement_admin_list_item::content_description)),
                                         boost::json::value_to<int32_t>(object.at(announcement_admin_list_item::priority_description)),
                                         boost::json::value_to<int64_t>(object.at(announcement_admin_list_item::publish_time_description)),
                                         underlying_cast_enum<announcement_status>(boost::json::value_to<int32_t>(object.at(announcement_admin_list_item::status_description))),
                                         boost::json::value_to<std::string>(object.at(announcement_admin_list_item::server_description)),
                                         boost::json::value_to<int64_t>(object.at(announcement_admin_list_item::create_time_description)),
                                         boost::json::value_to<int64_t>(object.at(announcement_admin_list_item::update_time_description)) };
}

void celeritas::tag_invoke(boost::json::value_from_tag, announcement_admin_list_item::json_value& value, const announcement_admin_list_item& item)
{
    value = {
        { announcement_admin_list_item::id_description, std::to_string(item.get_id()) },
        { announcement_admin_list_item::app_id_description, item.get_app_id() },
        { announcement_admin_list_item::title_description, item.get_title() },
        { announcement_admin_list_item::tag_description, item.get_tag() },
        { announcement_admin_list_item::banner_url_description, item.get_banner_url() },
        { announcement_admin_list_item::content_description, item.get_content() },
        { announcement_admin_list_item::priority_description, item.get_priority() },
        { announcement_admin_list_item::publish_time_description, item.get_publish_time() },
        { announcement_admin_list_item::status_description, enum_cast_underlying(item.get_status()) },
        { announcement_admin_list_item::server_description, item.get_server() },
        { announcement_admin_list_item::create_time_description, item.get_create_time() },
        { announcement_admin_list_item::update_time_description, item.get_update_time() }
    };
}
