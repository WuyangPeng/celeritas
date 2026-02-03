#pragma once

namespace celeritas
{
    class create_account;
    class create_user;
    class service_login;

    enum class player_state_type;
    enum class player_component_type;

    class player_manager;
    class player_state;
    class player_component;
    class player_null_component;

    class player_user_component;

    class player_role_component;
    class change_name;

    class player_online_component;
    class heartbeat;

    class debug_base;
    class add_item;
    class player_debug_component;

    class item_selected;
    class player_time_component;
    class player_time_refresh_key;

    enum class time_refresh_type;

    class player_red_dot_component;
    class red_dot_node;

    class player_item_component;
    class item_info;
    class item_container;

    class develop_level;
    class develop_reset;
    class player_develop_component;

    class mail_sync;
    class mail_read;
    class mail_collect_attachment;
    class mail_delete;
    class mail_collect_all_attachments;
    class mail_delete_all_read;
    class player_mail_component;

    class player_task_component;

    class player_activity_component;

    class player_attribute_component;

    class player_instance_component;

    class player_finish_component;

    class server_mail_manager;
}