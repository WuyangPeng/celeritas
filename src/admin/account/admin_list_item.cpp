// 创建时间：2026-07-30
// 修改时间：2026-07-30
// 审核时间：2026-08-07

#include "admin_list_item.h"
#include "common/core/enum_cast.h"

celeritas::admin_list_item::admin_list_item(std::string account_id,
                                            std::string username,
                                            const int64_t app_id,
                                            const int32_t role,
                                            std::string nickname,
                                            std::string avatar,
                                            std::string creator_account_id,
                                            const admin_status_type status,
                                            const int64_t create_time,
                                            const int64_t update_time,
                                            const int64_t last_login_time,
                                            std::string last_login_ip)
    : account_id_{ std::move(account_id) },
      username_{ std::move(username) },
      app_id_{ app_id },
      role_{ role },
      nickname_{ std::move(nickname) },
      avatar_{ std::move(avatar) },
      creator_account_id_{ std::move(creator_account_id) },
      status_{ status },
      create_time_{ create_time },
      update_time_{ update_time },
      last_login_time_{ last_login_time },
      last_login_ip_{ std::move(last_login_ip) }
{
}

const std::string& celeritas::admin_list_item::get_account_id() const noexcept
{
    return account_id_;
}

const std::string& celeritas::admin_list_item::get_username() const noexcept
{
    return username_;
}

int64_t celeritas::admin_list_item::get_app_id() const noexcept
{
    return app_id_;
}

int32_t celeritas::admin_list_item::get_role() const noexcept
{
    return role_;
}

const std::string& celeritas::admin_list_item::get_nickname() const noexcept
{
    return nickname_;
}

const std::string& celeritas::admin_list_item::get_avatar() const noexcept
{
    return avatar_;
}

const std::string& celeritas::admin_list_item::get_creator_account_id() const noexcept
{
    return creator_account_id_;
}

celeritas::admin_status_type celeritas::admin_list_item::get_status() const noexcept
{
    return status_;
}

int64_t celeritas::admin_list_item::get_create_time() const noexcept
{
    return create_time_;
}

int64_t celeritas::admin_list_item::get_update_time() const noexcept
{
    return update_time_;
}

int64_t celeritas::admin_list_item::get_last_login_time() const noexcept
{
    return last_login_time_;
}

const std::string& celeritas::admin_list_item::get_last_login_ip() const noexcept
{
    return last_login_ip_;
}

celeritas::admin_list_item celeritas::tag_invoke(admin_list_item_tag, const admin_list_item::json_value& value)
{
    const auto& object = value.as_object();

    return admin_list_item{ boost::json::value_to<std::string>(object.at(admin_list_item::account_id_description)),
                            boost::json::value_to<std::string>(object.at(admin_list_item::username_description)),
                            boost::json::value_to<int64_t>(object.at(admin_list_item::app_id_description)),
                            boost::json::value_to<int32_t>(object.at(admin_list_item::role_description)),
                            boost::json::value_to<std::string>(object.at(admin_list_item::nickname_description)),
                            boost::json::value_to<std::string>(object.at(admin_list_item::avatar_description)),
                            boost::json::value_to<std::string>(object.at(admin_list_item::creator_account_id_description)),
                            underlying_cast_enum<admin_status_type>(boost::json::value_to<int32_t>(object.at(admin_list_item::status_description))),
                            boost::json::value_to<int64_t>(object.at(admin_list_item::create_time_description)),
                            boost::json::value_to<int64_t>(object.at(admin_list_item::update_time_description)),
                            boost::json::value_to<int64_t>(object.at(admin_list_item::last_login_time_description)),
                            boost::json::value_to<std::string>(object.at(admin_list_item::last_login_ip_description)) };
}

void celeritas::tag_invoke(boost::json::value_from_tag, admin_list_item::json_value& value, const admin_list_item& item)
{
    value = {
        { admin_list_item::account_id_description, item.get_account_id() },
        { admin_list_item::username_description, item.get_username() },
        { admin_list_item::app_id_description, item.get_app_id() },
        { admin_list_item::role_description, item.get_role() },
        { admin_list_item::nickname_description, item.get_nickname() },
        { admin_list_item::avatar_description, item.get_avatar() },
        { admin_list_item::creator_account_id_description, item.get_creator_account_id() },
        { admin_list_item::status_description, enum_cast_underlying(item.get_status()) },
        { admin_list_item::create_time_description, item.get_create_time() },
        { admin_list_item::update_time_description, item.get_update_time() },
        { admin_list_item::last_login_time_description, item.get_last_login_time() },
        { admin_list_item::last_login_ip_description, item.get_last_login_ip() }
    };
}
