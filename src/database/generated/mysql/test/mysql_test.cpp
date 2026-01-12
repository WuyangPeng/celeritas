// 此文件是自动生成，请勿手动修改。

#include "mysql_test.h"
#include "config/basic/database_type.h"
#include "database/basic/basis_database.tpp"
#include "database/basic/database_change_type.h"
#include "database/basic/database_entity_change.tpp"
#include "database/entity/database_entity.h"
#include "database/entity/entity.tpp"

celeritas::mysql_test::mysql_test(const database_entity_change& entity)
    : base_type{ entity },
      user_id_{ entity.get_value<database_data_type::int64_type>(user_id_describe) },
      chapter_id_{ entity.get_value<database_data_type::int32_type>(chapter_id_describe) },
      chapter_name_{ entity.get_value<database_data_type::string_type>(chapter_name_describe) },
      chance_winning_{ entity.get_value<database_data_type::double_type>(chance_winning_describe) },
      winning_{ entity.get_value<database_data_type::bool_type>(winning_describe) },
      currency_{ entity.get_value<database_data_type::int64_count_type>(currency_describe) },
      count_{ entity.get_value<database_data_type::int32_count_type>(count_describe, 1) }
{
}

celeritas::mysql_test::mysql_test(const database_type database_type, const database_entity_change& entity)
    : base_type{ database_type, entity },
      user_id_{ entity.get_value<database_data_type::int64_type>(user_id_describe) },
      chapter_id_{ entity.get_value<database_data_type::int32_type>(chapter_id_describe) },
      chapter_name_{ entity.get_value<database_data_type::string_type>(chapter_name_describe) },
      chance_winning_{ entity.get_value<database_data_type::double_type>(chance_winning_describe) },
      winning_{ entity.get_value<database_data_type::bool_type>(winning_describe) },
      currency_{ entity.get_value<database_data_type::int64_count_type>(currency_describe) },
      count_{ entity.get_value<database_data_type::int32_count_type>(count_describe, 1) }
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
    }
}

celeritas::mysql_test::mysql_test(const database_type database_type, traits::param_type::int64_type user_id)
    : base_type{ database_type, database_name, get_key_basis_database_container(user_id) },
      user_id_{ user_id },
      chapter_id_{ traits::int32_type{} },
      chapter_name_{ traits::string_type{} },
      chance_winning_{ traits::double_type{} },
      winning_{ traits::bool_type{} },
      currency_{ traits::int64_count_type{} },
      count_{ traits::int32_count_type{ 1 } }
{
    add_modify(user_id_describe, user_id);
}

celeritas::traits::int64_type celeritas::mysql_test::get_user_id() const noexcept
{
    return user_id_.get_value();
}

celeritas::traits::int32_type celeritas::mysql_test::get_chapter_id() const noexcept
{
    return chapter_id_.get_value();
}

celeritas::traits::string_type celeritas::mysql_test::get_chapter_name() const
{
    return chapter_name_.get_value();
}

celeritas::traits::double_type celeritas::mysql_test::get_chance_winning() const noexcept
{
    return chance_winning_.get_value();
}

celeritas::traits::bool_type celeritas::mysql_test::is_winning() const noexcept
{
    return winning_.get_value();
}

celeritas::traits::int64_count_type celeritas::mysql_test::get_currency() const noexcept
{
    return currency_.get_value();
}

celeritas::traits::int32_count_type celeritas::mysql_test::get_count() const noexcept
{
    return count_.get_value();
}

void celeritas::mysql_test::set_user_id(traits::param_type::int64_type user_id)
{
    if (user_id != get_user_id())
    {
        user_id_.set_value(user_id);

        add_modify(user_id_describe, get_user_id());
    }
}

void celeritas::mysql_test::set_chapter_id(traits::param_type::int32_type chapter_id)
{
    if (chapter_id != get_chapter_id())
    {
        chapter_id_.set_value(chapter_id);

        add_modify(chapter_id_describe, get_chapter_id());
    }
}

void celeritas::mysql_test::set_chapter_name(traits::param_type::string_type chapter_name)
{
    if (chapter_name != get_chapter_name())
    {
        chapter_name_.set_value(chapter_name);

        add_modify(chapter_name_describe, get_chapter_name());
    }
}

void celeritas::mysql_test::set_chance_winning(traits::param_type::double_type chance_winning)
{
    if (chance_winning != get_chance_winning())
    {
        chance_winning_.set_value(chance_winning);

        add_modify(chance_winning_describe, get_chance_winning());
    }
}

void celeritas::mysql_test::set_winning(traits::param_type::bool_type winning)
{
    if (winning != is_winning())
    {
        winning_.set_value(winning);

        add_modify(winning_describe, is_winning());
    }
}

void celeritas::mysql_test::set_currency(traits::param_type::int64_count_type currency)
{
    if (currency != get_currency())
    {
        currency_.set_value(currency);

        add_modify(currency_describe, get_currency());
    }
}

void celeritas::mysql_test::set_count(traits::param_type::int32_count_type count)
{
    if (count != get_count())
    {
        count_.set_value(count);

        add_modify(count_describe, get_count());
    }
}

void celeritas::mysql_test::modify_currency(traits::param_type::int64_count_type currency)
{
    currency_.modify_value(currency);

    add_modify(currency_describe, get_currency());
}

void celeritas::mysql_test::modify_count(traits::param_type::int32_count_type count)
{
    count_.modify_value(count);

    add_modify(count_describe, get_count());
}

const celeritas::database_entity::database_field_container& celeritas::mysql_test::get_database_field_container()
{
    static const database_field_container field_name_container{ decltype(user_id_)::get_database_field(),
                                                                decltype(chapter_id_)::get_database_field(),
                                                                decltype(chapter_name_)::get_database_field(),
                                                                decltype(chance_winning_)::get_database_field(),
                                                                decltype(winning_)::get_database_field(),
                                                                decltype(currency_)::get_database_field(),
                                                                decltype(count_)::get_database_field() };

    return field_name_container;
}

celeritas::mysql_test::const_database_entity_change_shared_ptr celeritas::mysql_test::get_select(const database_type database_type)
{
    static const auto result = std::make_shared<database_entity_change>(database_type,
                                                                        database_name,
                                                                        database_change_type::select_type);

    return result;
}

celeritas::mysql_test::const_database_entity_change_shared_ptr celeritas::mysql_test::get_select(const database_type database_type, traits::param_type::int64_type user_id)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    get_key_basis_database_container(user_id));
}

celeritas::mysql_test::const_database_entity_change_shared_ptr celeritas::mysql_test::get_select(const database_type database_type, const const_basis_database_container_shared_ptr& key)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    key);
}

celeritas::mysql_test::const_basis_database_container_shared_ptr celeritas::mysql_test::get_key_basis_database_container(traits::param_type::int64_type user_id)
{
    return std::make_shared<basis_database_container>(basis_database{ user_id_describe, user_id });
}