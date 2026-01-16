#include "check_helper.h"

void celeritas::check_properties_data(const traits::document_type& lhs, const traits::document_type& rhs)
{
    const auto lhs_loaded_properties = properties_data::from_document(lhs);
    const auto rhs_loaded_properties = properties_data::from_document(rhs);

    check_properties_data(lhs_loaded_properties, rhs_loaded_properties);
}

void celeritas::check_properties_data(const properties_data& lhs, const properties_data& rhs)
{
    BOOST_CHECK_EQUAL(lhs.get_int64_value(), rhs.get_int64_value());
    BOOST_CHECK_EQUAL(lhs.get_string_value(), rhs.get_string_value());
    check_array(lhs.get_string_array_value(), rhs.get_string_array_value());
    BOOST_CHECK_EQUAL(lhs.get_int32_value(), rhs.get_int32_value());
    BOOST_CHECK_EQUAL(lhs.get_int32_count_value(), rhs.get_int32_count_value());
    check_array(lhs.get_int32_array_value(), rhs.get_int32_array_value());
    BOOST_CHECK_EQUAL(lhs.get_int64_count_value(), rhs.get_int64_count_value());
    check_array(lhs.get_int64_array_value(), rhs.get_int64_array_value());
    BOOST_WARN_CLOSE(lhs.get_double_value(), rhs.get_double_value(), 0.000001);
    check_array(lhs.get_double_array_value(), rhs.get_double_array_value());
    BOOST_CHECK_EQUAL(lhs.get_bool_value(), rhs.get_bool_value());
    BOOST_CHECK(lhs.get_document_value() == rhs.get_document_value());
    BOOST_CHECK(lhs.get_document_array_value() == rhs.get_document_array_value());
}

void celeritas::check_logs_data(const traits::document_array_type& lhs, const traits::document_array_type& rhs)
{
    BOOST_CHECK_EQUAL(lhs.size(), lhs.size());
    for (auto i = 0; i < lhs.size(); ++i)
    {
        const auto lhs_loaded_log = logs_data::from_document(lhs.at(i));
        const auto rhs_loaded_log = logs_data::from_document(rhs.at(i));

        check_logs_data(lhs_loaded_log, rhs_loaded_log);
    }
}

void celeritas::check_logs_data(const logs_data& lhs, const logs_data& rhs)
{
    BOOST_CHECK_EQUAL(lhs.get_int64_value(), rhs.get_int64_value());
    BOOST_CHECK_EQUAL(lhs.get_string_value(), rhs.get_string_value());
    check_array(lhs.get_string_array_value(), rhs.get_string_array_value());
    BOOST_CHECK_EQUAL(lhs.get_int32_value(), rhs.get_int32_value());
    BOOST_CHECK_EQUAL(lhs.get_int32_count_value(), rhs.get_int32_count_value());
    check_array(lhs.get_int32_array_value(), rhs.get_int32_array_value());
    BOOST_CHECK_EQUAL(lhs.get_int64_count_value(), rhs.get_int64_count_value());
    check_array(lhs.get_int64_array_value(), rhs.get_int64_array_value());
    BOOST_WARN_CLOSE(lhs.get_double_value(), rhs.get_double_value(), 0.000001);
    check_array(lhs.get_double_array_value(), rhs.get_double_array_value());
    BOOST_CHECK_EQUAL(lhs.get_bool_value(), rhs.get_bool_value());
    BOOST_CHECK(lhs.get_document_value() == rhs.get_document_value());
    BOOST_CHECK(lhs.get_document_array_value() == rhs.get_document_array_value());
}