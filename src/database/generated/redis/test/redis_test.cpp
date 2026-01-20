// 此文件是自动生成，请勿手动修改。

#include "redis_test.h"
#include "config/basic/database_type.h"
#include "database/basic/basis_database.tpp"
#include "database/basic/database_change_type.h"
#include "database/basic/database_entity_change.tpp"
#include "database/entity/database_entity.h"
#include "database/entity/entity.tpp"

celeritas::redis_test::redis_test(const database_entity_change& entity)
    : base_type{ entity.get_database_entity_change(user_id_describe)  },
      user_id_{ entity.get_value<database_data_type::int64_type>(user_id_describe) },
      chapter_id_{ entity.get_value<database_data_type::int32_type>(chapter_id_describe) },
      chapter_name_{ entity.get_value<database_data_type::string_type>(chapter_name_describe) },
      chance_winning_{ entity.get_value<database_data_type::double_type>(chance_winning_describe) },
      winning_{ entity.get_value<database_data_type::bool_type>(winning_describe) },
      currency_{ entity.get_value<database_data_type::int64_count_type>(currency_describe) },
      count_{ entity.get_value<database_data_type::int32_count_type>(count_describe, 1) },
      tags_{ entity.get_value<database_data_type::string_array_type>(tags_describe) },
      category_index_{ entity.get_value<database_data_type::int32_array_type>(category_index_describe) },
      related_index_{ entity.get_value<database_data_type::int64_array_type>(related_index_describe) },
      ratios_{ entity.get_value<database_data_type::double_array_type>(ratios_describe) },
      attachment_{ entity.get_value<database_data_type::byte_array_type>(attachment_describe) },
      properties_{ entity.get_value<database_data_type::document_type>(properties_describe) },
      logs_{ entity.get_value<database_data_type::document_array_type>(logs_describe) }
{
}

celeritas::redis_test::redis_test(const database_type database_type, const database_entity_change& entity)
    : base_type{ database_type, entity.get_database_entity_change(user_id_describe) },
      user_id_{ entity.get_value<database_data_type::int64_type>(user_id_describe) },
      chapter_id_{ entity.get_value<database_data_type::int32_type>(chapter_id_describe) },
      chapter_name_{ entity.get_value<database_data_type::string_type>(chapter_name_describe) },
      chance_winning_{ entity.get_value<database_data_type::double_type>(chance_winning_describe) },
      winning_{ entity.get_value<database_data_type::bool_type>(winning_describe) },
      currency_{ entity.get_value<database_data_type::int64_count_type>(currency_describe) },
      count_{ entity.get_value<database_data_type::int32_count_type>(count_describe, 1) },
      tags_{ entity.get_value<database_data_type::string_array_type>(tags_describe) },
      category_index_{ entity.get_value<database_data_type::int32_array_type>(category_index_describe) },
      related_index_{ entity.get_value<database_data_type::int64_array_type>(related_index_describe) },
      ratios_{ entity.get_value<database_data_type::double_array_type>(ratios_describe) },
      attachment_{ entity.get_value<database_data_type::byte_array_type>(attachment_describe) },
      properties_{ entity.get_value<database_data_type::document_type>(properties_describe) },
      logs_{ entity.get_value<database_data_type::document_array_type>(logs_describe) }
{
    if (database_type != entity.get_database_type())
    {
        add_modify(user_id_describe, get_user_id());
        add_modify(chapter_id_describe, get_chapter_id());
        add_modify(chapter_name_describe, get_chapter_name());
        add_modify(chance_winning_describe, get_chance_winning());
        add_modify(winning_describe, is_winning());
        add_modify(currency_describe, get_currency());
        add_modify(count_describe, get_count());
        add_modify(tags_describe, get_tags());
        add_modify(category_index_describe, get_category_index());
        add_modify(related_index_describe, get_related_index());
        add_modify(ratios_describe, get_ratios());
        add_modify(attachment_describe, get_attachment());
        add_modify(properties_describe, get_properties());
        add_modify(logs_describe, get_logs());
    }
}

celeritas::redis_test::redis_test(const database_type database_type, traits::param_type::int64_type user_id)
    : base_type{ database_type, database_name, get_key_basis_database_container(user_id) },
      user_id_{ user_id },
      chapter_id_{ traits::int32_type{} },
      chapter_name_{ traits::string_type{} },
      chance_winning_{ traits::double_type{} },
      winning_{ traits::bool_type{} },
      currency_{ traits::int64_count_type{} },
      count_{ traits::int32_count_type{ 1 } },
      tags_{ traits::string_array_type{} },
      category_index_{ traits::int32_array_type{} },
      related_index_{ traits::int64_array_type{} },
      ratios_{ traits::double_array_type{} },
      attachment_{ traits::byte_array_type{} },
      properties_{ traits::document_type{} },
      logs_{ traits::document_array_type{} }
{
    add_modify(user_id_describe, user_id);
}

celeritas::traits::int64_type celeritas::redis_test::get_user_id() const noexcept
{
    return user_id_.get_value();
}

celeritas::traits::int32_type celeritas::redis_test::get_chapter_id() const noexcept
{
    return chapter_id_.get_value();
}

celeritas::traits::string_type celeritas::redis_test::get_chapter_name() const
{
    return chapter_name_.get_value();
}

celeritas::traits::double_type celeritas::redis_test::get_chance_winning() const noexcept
{
    return chance_winning_.get_value();
}

celeritas::traits::bool_type celeritas::redis_test::is_winning() const noexcept
{
    return winning_.get_value();
}

celeritas::traits::int64_count_type celeritas::redis_test::get_currency() const noexcept
{
    return currency_.get_value();
}

celeritas::traits::int32_count_type celeritas::redis_test::get_count() const noexcept
{
    return count_.get_value();
}

celeritas::traits::string_array_type celeritas::redis_test::get_tags() const
{
    return tags_.get_value();
}

celeritas::traits::int32_array_type celeritas::redis_test::get_category_index() const
{
    return category_index_.get_value();
}

celeritas::traits::int64_array_type celeritas::redis_test::get_related_index() const
{
    return related_index_.get_value();
}

celeritas::traits::double_array_type celeritas::redis_test::get_ratios() const
{
    return ratios_.get_value();
}

celeritas::traits::byte_array_type celeritas::redis_test::get_attachment() const
{
    return attachment_.get_value();
}

celeritas::traits::document_type celeritas::redis_test::get_properties() const
{
    return properties_.get_value();
}

celeritas::traits::document_array_type celeritas::redis_test::get_logs() const
{
    return logs_.get_value();
}

void celeritas::redis_test::set_user_id(traits::param_type::int64_type user_id)
{
    if (user_id != get_user_id())
    {
        user_id_.set_value(user_id);

        add_modify(user_id_describe, get_user_id());
    }
}

void celeritas::redis_test::set_chapter_id(traits::param_type::int32_type chapter_id)
{
    if (chapter_id != get_chapter_id())
    {
        chapter_id_.set_value(chapter_id);

        add_modify(chapter_id_describe, get_chapter_id());
    }
}

void celeritas::redis_test::set_chapter_name(traits::param_type::string_type chapter_name)
{
    if (chapter_name != get_chapter_name())
    {
        chapter_name_.set_value(chapter_name);

        add_modify(chapter_name_describe, get_chapter_name());
    }
}

void celeritas::redis_test::set_chance_winning(traits::param_type::double_type chance_winning)
{
    if (chance_winning != get_chance_winning())
    {
        chance_winning_.set_value(chance_winning);

        add_modify(chance_winning_describe, get_chance_winning());
    }
}

void celeritas::redis_test::set_winning(traits::param_type::bool_type winning)
{
    if (winning != is_winning())
    {
        winning_.set_value(winning);

        add_modify(winning_describe, is_winning());
    }
}

void celeritas::redis_test::set_currency(traits::param_type::int64_count_type currency)
{
    if (currency != get_currency())
    {
        currency_.set_value(currency);

        add_modify(currency_describe, get_currency());
    }
}

void celeritas::redis_test::set_count(traits::param_type::int32_count_type count)
{
    if (count != get_count())
    {
        count_.set_value(count);

        add_modify(count_describe, get_count());
    }
}

void celeritas::redis_test::set_tags(traits::param_type::string_array_type tags)
{
    if (tags != get_tags())
    {
        tags_.set_value(tags);

        add_modify(tags_describe, get_tags());
    }
}

void celeritas::redis_test::set_category_index(traits::param_type::int32_array_type category_index)
{
    if (category_index != get_category_index())
    {
        category_index_.set_value(category_index);

        add_modify(category_index_describe, get_category_index());
    }
}

void celeritas::redis_test::set_related_index(traits::param_type::int64_array_type related_index)
{
    if (related_index != get_related_index())
    {
        related_index_.set_value(related_index);

        add_modify(related_index_describe, get_related_index());
    }
}

void celeritas::redis_test::set_ratios(traits::param_type::double_array_type ratios)
{
    if (ratios != get_ratios())
    {
        ratios_.set_value(ratios);

        add_modify(ratios_describe, get_ratios());
    }
}

void celeritas::redis_test::set_attachment(traits::param_type::byte_array_type attachment)
{
    if (attachment != get_attachment())
    {
        attachment_.set_value(attachment);

        add_modify(attachment_describe, get_attachment());
    }
}

void celeritas::redis_test::set_properties(traits::param_type::document_type properties)
{
    if (properties != get_properties())
    {
        properties_.set_value(properties);

        add_modify(properties_describe, get_properties());
    }
}

void celeritas::redis_test::set_logs(traits::param_type::document_array_type logs)
{
    if (logs != get_logs())
    {
        logs_.set_value(logs);

        add_modify(logs_describe, get_logs());
    }
}

void celeritas::redis_test::modify_currency(traits::param_type::int64_count_type currency)
{
    currency_.modify_value(currency);

    add_modify(currency_describe, get_currency());
}

void celeritas::redis_test::modify_count(traits::param_type::int32_count_type count)
{
    count_.modify_value(count);

    add_modify(count_describe, get_count());
}

void celeritas::redis_test::set_tags(const int index, traits::param_type::string_array_element_type tags)
{
    if (tags_.set_value(index, tags))
    {
        add_modify(tags_describe, get_tags());
    }
}

void celeritas::redis_test::add_tags(traits::param_type::string_array_element_type tags)
{
    tags_.add_value(tags);

    add_modify(tags_describe, get_tags());
}

void celeritas::redis_test::remove_tags(const int index)
{
    tags_.remove_value(index);

    add_modify(tags_describe, get_tags());
}

void celeritas::redis_test::set_category_index(const int index, traits::param_type::int32_array_element_type category_index)
{
    if (category_index_.set_value(index, category_index))
    {
        add_modify(category_index_describe, get_category_index());
    }
}

void celeritas::redis_test::add_category_index(traits::param_type::int32_array_element_type category_index)
{
    category_index_.add_value(category_index);

    add_modify(category_index_describe, get_category_index());
}

void celeritas::redis_test::remove_category_index(const int index)
{
    category_index_.remove_value(index);

    add_modify(category_index_describe, get_category_index());
}

void celeritas::redis_test::set_related_index(const int index, traits::param_type::int64_array_element_type related_index)
{
    if (related_index_.set_value(index, related_index))
    {
        add_modify(related_index_describe, get_related_index());
    }
}

void celeritas::redis_test::add_related_index(traits::param_type::int64_array_element_type related_index)
{
    related_index_.add_value(related_index);

    add_modify(related_index_describe, get_related_index());
}

void celeritas::redis_test::remove_related_index(const int index)
{
    related_index_.remove_value(index);

    add_modify(related_index_describe, get_related_index());
}

void celeritas::redis_test::set_ratios(const int index, traits::param_type::double_array_element_type ratios)
{
    if (ratios_.set_value(index, ratios))
    {
        add_modify(ratios_describe, get_ratios());
    }
}

void celeritas::redis_test::add_ratios(traits::param_type::double_array_element_type ratios)
{
    ratios_.add_value(ratios);

    add_modify(ratios_describe, get_ratios());
}

void celeritas::redis_test::remove_ratios(const int index)
{
    ratios_.remove_value(index);

    add_modify(ratios_describe, get_ratios());
}

void celeritas::redis_test::set_attachment(const int index, traits::param_type::byte_array_element_type attachment)
{
    if (attachment_.set_value(index, attachment))
    {
        add_modify(attachment_describe, get_attachment());
    }
}

void celeritas::redis_test::add_attachment(traits::param_type::byte_array_element_type attachment)
{
    attachment_.add_value(attachment);

    add_modify(attachment_describe, get_attachment());
}

void celeritas::redis_test::remove_attachment(const int index)
{
    attachment_.remove_value(index);

    add_modify(attachment_describe, get_attachment());
}

void celeritas::redis_test::set_logs(const int index, traits::param_type::document_array_element_type logs)
{
    if (logs_.set_value(index, logs))
    {
        add_modify(logs_describe, get_logs());
    }
}

void celeritas::redis_test::add_logs(traits::param_type::document_array_element_type logs)
{
    logs_.add_value(logs);

    add_modify(logs_describe, get_logs());
}

void celeritas::redis_test::remove_logs(const int index)
{
    logs_.remove_value(index);

    add_modify(logs_describe, get_logs());
}

const celeritas::database_entity::database_field_container& celeritas::redis_test::get_database_field_container()
{
    static const database_field_container field_name_container{ decltype(user_id_)::get_database_field(),
                                                                decltype(chapter_id_)::get_database_field(),
                                                                decltype(chapter_name_)::get_database_field(),
                                                                decltype(chance_winning_)::get_database_field(),
                                                                decltype(winning_)::get_database_field(),
                                                                decltype(currency_)::get_database_field(),
                                                                decltype(count_)::get_database_field(),
                                                                decltype(tags_)::get_database_field(),
                                                                decltype(category_index_)::get_database_field(),
                                                                decltype(related_index_)::get_database_field(),
                                                                decltype(ratios_)::get_database_field(),
                                                                decltype(attachment_)::get_database_field(),
                                                                decltype(properties_)::get_database_field(),
                                                                decltype(logs_)::get_database_field() };

    return field_name_container;
}

celeritas::redis_test::const_database_entity_change_shared_ptr celeritas::redis_test::get_select(const database_type database_type)
{
    static const auto result = std::make_shared<database_entity_change>(database_type,
                                                                        database_name,
                                                                        database_change_type::select_type);

    return result;
}

celeritas::redis_test::const_database_entity_change_shared_ptr celeritas::redis_test::get_select(const database_type database_type, traits::param_type::int64_type user_id)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    get_key_basis_database_container(user_id));
}

celeritas::redis_test::const_database_entity_change_shared_ptr celeritas::redis_test::get_select(const database_type database_type, const const_basis_database_container_shared_ptr& key)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    key);
}

celeritas::redis_test::const_basis_database_container_shared_ptr celeritas::redis_test::get_key_basis_database_container(traits::param_type::int64_type user_id)
{
    return std::make_shared<basis_database_container>(basis_database{ user_id_describe, user_id });
}