#include "common/core/celeritas_error.h"
#include "config/basic/database_type.h"
#include "database/basic/database_change_type.h"
#include "database/basic/database_data_type.h"
#include "database/basic/database_entity_change.h"
#include "database/basic/database_field.h"
#include "database/basic/database_select_options.h"
#include "database/session/detail/mysql_statement_generator.h"

#include <boost/test/unit_test.hpp>

namespace
{
    celeritas::database_select_options::database_field_container make_fields()
    {
        return {
            celeritas::database_field{ "account_id", celeritas::database_data_type::int64_type },
            celeritas::database_field{ "app_id", celeritas::database_data_type::int64_type },
            celeritas::database_field{ "username", celeritas::database_data_type::string_type },
        };
    }
}

BOOST_AUTO_TEST_SUITE(database_select_options_suite)

    BOOST_AUTO_TEST_CASE(test_validate_rejects_unknown_order_by_field)
    {
        celeritas::database_select_options options{};
        options.add_order_by("unknown_field");

        BOOST_CHECK_THROW(options.validate(make_fields()), celeritas::celeritas_error);
    }

    BOOST_AUTO_TEST_CASE(test_validate_rejects_offset_without_limit)
    {
        celeritas::database_select_options options{};
        options.set_offset(10);

        BOOST_CHECK_THROW(options.validate(make_fields()), celeritas::celeritas_error);
    }

    BOOST_AUTO_TEST_CASE(test_validate_accepts_valid_options)
    {
        celeritas::database_select_options options{};
        options.add_order_by("account_id", celeritas::sort_order::asc);
        options.set_limit(10);
        options.set_offset(20);

        BOOST_CHECK_NO_THROW(options.validate(make_fields()));
    }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(mysql_statement_generator_select_page_suite)

    BOOST_AUTO_TEST_CASE(test_generate_select_with_where_order_limit_offset)
    {
        const auto key = std::make_shared<celeritas::basis_database_container>(celeritas::basis_database{ "app_id", int64_t{ 100 } });
        const auto database = std::make_shared<const celeritas::database_entity_change>(celeritas::database_type::mysql,
                                                                                       "admin_account",
                                                                                       celeritas::database_change_type::select_type,
                                                                                       key);

        celeritas::database_select_options options{};
        options.add_order_by("account_id", celeritas::sort_order::asc);
        options.set_limit(10);
        options.set_offset(20);

        const auto sql = celeritas::mysql_statement_generator::generate_select_statement(make_fields(), database, options);

        BOOST_CHECK_EQUAL(sql,
                          "SELECT `account_id` , `app_id` , `username` FROM `admin_account` WHERE `app_id` = 100 ORDER BY `account_id` ASC LIMIT 10 OFFSET 20");
    }

    BOOST_AUTO_TEST_CASE(test_generate_select_with_desc_order)
    {
        const auto database = std::make_shared<const celeritas::database_entity_change>(celeritas::database_type::mysql,
                                                                                       "admin_account",
                                                                                       celeritas::database_change_type::select_type);

        celeritas::database_select_options options{};
        options.add_order_by("account_id", celeritas::sort_order::desc);
        options.set_limit(5);

        const auto sql = celeritas::mysql_statement_generator::generate_select_statement(make_fields(), database, options);

        BOOST_CHECK_EQUAL(sql,
                          "SELECT `account_id` , `app_id` , `username` FROM `admin_account`  ORDER BY `account_id` DESC LIMIT 5");
    }

    BOOST_AUTO_TEST_CASE(test_generate_count_with_where)
    {
        const auto key = std::make_shared<celeritas::basis_database_container>(celeritas::basis_database{ "app_id", int64_t{ 100 } });
        const auto database = std::make_shared<const celeritas::database_entity_change>(celeritas::database_type::mysql,
                                                                                       "admin_account",
                                                                                       celeritas::database_change_type::select_type,
                                                                                       key);

        const auto sql = celeritas::mysql_statement_generator::generate_count_statement(database);

        BOOST_CHECK_EQUAL(sql, "SELECT COUNT(*) FROM `admin_account` WHERE `app_id` = 100");
    }

    BOOST_AUTO_TEST_CASE(test_generate_count_without_where)
    {
        const auto database = std::make_shared<const celeritas::database_entity_change>(celeritas::database_type::mysql,
                                                                                       "admin_account",
                                                                                       celeritas::database_change_type::select_type);

        const auto sql = celeritas::mysql_statement_generator::generate_count_statement(database);

        BOOST_CHECK_EQUAL(sql, "SELECT COUNT(*) FROM `admin_account` ");
    }

BOOST_AUTO_TEST_SUITE_END()
