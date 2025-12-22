#include "player_item_component.h"

celeritas::player_item_component::player_item_component(player_state* player_state) noexcept
    : base_type{ get_player_component_type(), player_state },
      database_{ player_state, this },
      document_{}
{
}

celeritas::player_component::void_awaitable_type celeritas::player_item_component::on_load_db()
{
    co_await database_.load_user_item();

    document_.set_item(database_.get_inventory_data());

    co_return;
}

celeritas::player_component::void_awaitable_type celeritas::player_item_component::save_db()
{
    co_await database_.save_db();

    co_return;
}

bool celeritas::player_item_component::is_modify() const
{
    return database_.is_modify();
}

void celeritas::player_item_component::change_item(const const_app_config_shared_ptr& app_config, const int template_id, const int64_t count)
{
    if (document_.change_item(app_config, template_id, count))
    {
        update_document();
    }
}

bool celeritas::player_item_component::can_consume_item(const int template_id, const int64_t count) const
{
    return document_.can_consume_item(template_id, count);
}

int64_t celeritas::player_item_component::get_count(const int template_id) const
{
    return document_.get_count(template_id);
}

bool celeritas::player_item_component::can_consume_item(const item_container& item) const
{
    return document_.can_consume_item(item);
}

void celeritas::player_item_component::change_item(const const_app_config_shared_ptr& app_config, const item_container& item)
{
    if (document_.change_item(app_config, item))
    {
        update_document();
    }
}

void celeritas::player_item_component::update_document()
{
    database_.set_item(document_.get_item());
}