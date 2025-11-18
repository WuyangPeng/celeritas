// 此文件是自动生成，请勿手动修改。

#include "test.h"
#include "config/database_type.h"
#include "database/basis_database_manager.h"
#include "database/database_change_type.h"
#include "database/entity.tpp"

celeritas::test celeritas::test::create(const basis_database_manager& entity, const database_type database_type, traits::param_type::int64_type user_id)
{
    return entity.is_modify() ? test{ entity } : test{ database_type, user_id };
}

celeritas::test::test(const basis_database_manager& entity)
    : base_type{ entity },
      user_id_{ entity.get_value<database_data_type::int64_type>(user_id_describe) },
      chapter_id_{ entity.get_value<database_data_type::int32_type>(chapter_id_describe) },
      chapter_name_{ entity.get_value<database_data_type::string_type>(chapter_name_describe) },
      chance_winning_{ entity.get_value<database_data_type::double_type>(chance_winning_describe) },
      winning_{ entity.get_value<database_data_type::bool_type>(winning_describe) },
      currency_{ entity.get_value<database_data_type::int64_count_type>(currency_describe) },
      count_{ entity.get_value<database_data_type::int32_count_type>(count_describe) }
{
}

celeritas::test::test(const database_type database_type, traits::param_type::int64_type user_id)
    : base_type{ database_type, database_name.data(), get_key_basis_database_container(user_id) },
      user_id_{ user_id },
      chapter_id_{ traits::int32_type{} },
      chapter_name_{ traits::string_type{} },
      chance_winning_{ traits::double_type{} },
      winning_{ traits::bool_type{} },
      currency_{ traits::int64_count_type{} },
      count_{ traits::int32_count_type{} }
{
    add_modify(user_id_describe, user_id);
}

celeritas::traits::int64_type celeritas::test::get_user_id() const noexcept
{
    return user_id_.get_value();
}

celeritas::traits::int32_type celeritas::test::get_chapter_id() const noexcept
{
    return chapter_id_.get_value();
}

celeritas::traits::string_type celeritas::test::get_chapter_name() const
{
    return chapter_name_.get_value();
}

celeritas::traits::double_type celeritas::test::get_chance_winning() const noexcept
{
    return chance_winning_.get_value();
}

celeritas::traits::bool_type celeritas::test::is_winning() const noexcept
{
    return winning_.get_value();
}

celeritas::traits::int64_count_type celeritas::test::get_currency() const noexcept
{
    return currency_.get_value();
}

celeritas::traits::int32_count_type celeritas::test::get_count() const noexcept
{
    return count_.get_value();
}

void celeritas::test::set_user_id(traits::param_type::int64_type user_id)
{
    if (user_id != get_user_id())
    {
        user_id_.set_value(user_id);

        add_modify(user_id_describe, get_user_id());
    }
}

void celeritas::test::set_chapter_id(traits::param_type::int32_type chapter_id)
{
    if (chapter_id != get_chapter_id())
    {
        chapter_id_.set_value(chapter_id);

        add_modify(chapter_id_describe, get_chapter_id());
    }
}

void celeritas::test::set_chapter_name(traits::param_type::string_type chapter_name)
{
    if (chapter_name != get_chapter_name())
    {
        chapter_name_.set_value(chapter_name);

        add_modify(chapter_name_describe, get_chapter_name());
    }
}

void celeritas::test::set_chance_winning(traits::param_type::double_type chance_winning)
{
    if (chance_winning != get_chance_winning())
    {
        chance_winning_.set_value(chance_winning);

        add_modify(chance_winning_describe, get_chance_winning());
    }
}

void celeritas::test::set_winning(traits::param_type::bool_type winning)
{
    if (winning != is_winning())
    {
        winning_.set_value(winning);

        add_modify(winning_describe, is_winning());
    }
}

void celeritas::test::set_currency(traits::param_type::int64_count_type currency)
{
    if (currency != get_currency())
    {
        currency_.set_value(currency);

        add_modify(currency_describe, get_currency());
    }
}

void celeritas::test::set_count(traits::param_type::int32_count_type count)
{
    if (count != get_count())
    {
        count_.set_value(count);

        add_modify(count_describe, get_count());
    }
}

void celeritas::test::modify_currency(traits::param_type::int64_count_type currency)
{
    currency_.modify_value(currency);

    add_modify(currency_describe, get_currency());
}

void celeritas::test::modify_count(traits::param_type::int32_count_type count)
{
    count_.modify_value(count);

    add_modify(count_describe, get_count());
}

const celeritas::database_entity::database_field_container& celeritas::test::get_database_field_container()
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

celeritas::test::basis_database_manager_const_hared_ptr celeritas::test::get_select(const database_type database_type, traits::param_type::int64_type user_id)
{
    return std::make_shared<basis_database_manager>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    get_key_basis_database_container(user_id));
}

celeritas::test::basis_database_manager_const_hared_ptr celeritas::test::get_select_all(const database_type database_type)
{
    static const auto result = std::make_shared<basis_database_manager>(database_type,
                                                                        database_name,
                                                                        database_change_type::select_type,
                                                                        basis_database_container{});

    return result;
}

celeritas::basis_database_container celeritas::test::get_key_basis_database_container(traits::param_type::int64_type user_id)
{
    basis_database_container basis_database_container{ basis_database_container::object_container{ basis_database{ user_id_describe, user_id } } };

    return basis_database_container;
}