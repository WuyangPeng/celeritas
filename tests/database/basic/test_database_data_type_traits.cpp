#include "database/basic/basis_database.h"
#include "database/basic/database_data_type_traits.h"

#include <boost/call_traits.hpp>
#include <boost/test/unit_test.hpp>

#include <cstdint>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

using namespace celeritas;

BOOST_AUTO_TEST_SUITE(database_data_type_traits_suite)

    BOOST_AUTO_TEST_CASE(test_traits_null_type)
    {
        using traits = database_data_type_traits<database_data_type::null_type>;

        BOOST_CHECK((std::is_same_v<traits::type, std::monostate>));
        BOOST_CHECK((std::is_same_v<traits::element_type, std::monostate>));
        BOOST_CHECK((std::is_same_v<traits::param_type, boost::call_traits<traits::type>::param_type>));
        BOOST_CHECK((std::is_same_v<traits::element_param_type, boost::call_traits<traits::element_type>::param_type>));
    }

    BOOST_AUTO_TEST_CASE(test_traits_string_type)
    {
        using traits = database_data_type_traits<database_data_type::string_type>;

        BOOST_CHECK((std::is_same_v<traits::type, std::string>));
        BOOST_CHECK((std::is_same_v<traits::element_type, std::string>));
        BOOST_CHECK((std::is_same_v<traits::param_type, boost::call_traits<traits::type>::param_type>));
        BOOST_CHECK((std::is_same_v<traits::element_param_type, boost::call_traits<traits::element_type>::param_type>));
    }

    BOOST_AUTO_TEST_CASE(test_traits_string_array_type)
    {
        using traits = database_data_type_traits<database_data_type::string_array_type>;

        BOOST_CHECK((std::is_same_v<traits::type, std::vector<std::string>>));
        BOOST_CHECK((std::is_same_v<traits::element_type, std::string>));
        BOOST_CHECK((std::is_same_v<traits::param_type, boost::call_traits<traits::type>::param_type>));
        BOOST_CHECK((std::is_same_v<traits::element_param_type, boost::call_traits<traits::element_type>::param_type>));
    }

    BOOST_AUTO_TEST_CASE(test_traits_int32_type)
    {
        using traits = database_data_type_traits<database_data_type::int32_type>;

        BOOST_CHECK((std::is_same_v<traits::type, int32_t>));
        BOOST_CHECK((std::is_same_v<traits::element_type, int32_t>));
        BOOST_CHECK((std::is_same_v<traits::param_type, boost::call_traits<traits::type>::param_type>));
        BOOST_CHECK((std::is_same_v<traits::element_param_type, boost::call_traits<traits::element_type>::param_type>));
    }

    BOOST_AUTO_TEST_CASE(test_traits_int32_count_type)
    {
        using traits = database_data_type_traits<database_data_type::int32_count_type>;

        BOOST_CHECK((std::is_same_v<traits::type, int32_t>));
        BOOST_CHECK((std::is_same_v<traits::element_type, int32_t>));
        BOOST_CHECK((std::is_same_v<traits::param_type, boost::call_traits<traits::type>::param_type>));
        BOOST_CHECK((std::is_same_v<traits::element_param_type, boost::call_traits<traits::element_type>::param_type>));
    }

    BOOST_AUTO_TEST_CASE(test_traits_int32_array_type)
    {
        using traits = database_data_type_traits<database_data_type::int32_array_type>;

        BOOST_CHECK((std::is_same_v<traits::type, std::vector<int32_t>>));
        BOOST_CHECK((std::is_same_v<traits::element_type, int32_t>));
        BOOST_CHECK((std::is_same_v<traits::param_type, boost::call_traits<traits::type>::param_type>));
        BOOST_CHECK((std::is_same_v<traits::element_param_type, boost::call_traits<traits::element_type>::param_type>));
    }

    BOOST_AUTO_TEST_CASE(test_traits_int64_type)
    {
        using traits = database_data_type_traits<database_data_type::int64_type>;

        BOOST_CHECK((std::is_same_v<traits::type, int64_t>));
        BOOST_CHECK((std::is_same_v<traits::element_type, int64_t>));
        BOOST_CHECK((std::is_same_v<traits::param_type, boost::call_traits<traits::type>::param_type>));
        BOOST_CHECK((std::is_same_v<traits::element_param_type, boost::call_traits<traits::element_type>::param_type>));
    }

    BOOST_AUTO_TEST_CASE(test_traits_int64_count_type)
    {
        using traits = database_data_type_traits<database_data_type::int64_count_type>;

        BOOST_CHECK((std::is_same_v<traits::type, int64_t>));
        BOOST_CHECK((std::is_same_v<traits::element_type, int64_t>));
        BOOST_CHECK((std::is_same_v<traits::param_type, boost::call_traits<traits::type>::param_type>));
        BOOST_CHECK((std::is_same_v<traits::element_param_type, boost::call_traits<traits::element_type>::param_type>));
    }

    BOOST_AUTO_TEST_CASE(test_traits_int64_array_type)
    {
        using traits = database_data_type_traits<database_data_type::int64_array_type>;

        BOOST_CHECK((std::is_same_v<traits::type, std::vector<int64_t>>));
        BOOST_CHECK((std::is_same_v<traits::element_type, int64_t>));
        BOOST_CHECK((std::is_same_v<traits::param_type, boost::call_traits<traits::type>::param_type>));
        BOOST_CHECK((std::is_same_v<traits::element_param_type, boost::call_traits<traits::element_type>::param_type>));
    }

    BOOST_AUTO_TEST_CASE(test_traits_double_type)
    {
        using traits = database_data_type_traits<database_data_type::double_type>;

        BOOST_CHECK((std::is_same_v<traits::type, double>));
        BOOST_CHECK((std::is_same_v<traits::element_type, double>));
        BOOST_CHECK((std::is_same_v<traits::param_type, boost::call_traits<traits::type>::param_type>));
        BOOST_CHECK((std::is_same_v<traits::element_param_type, boost::call_traits<traits::element_type>::param_type>));
    }

    BOOST_AUTO_TEST_CASE(test_traits_double_array_type)
    {
        using traits = database_data_type_traits<database_data_type::double_array_type>;

        BOOST_CHECK((std::is_same_v<traits::type, std::vector<double>>));
        BOOST_CHECK((std::is_same_v<traits::element_type, double>));
        BOOST_CHECK((std::is_same_v<traits::param_type, boost::call_traits<traits::type>::param_type>));
        BOOST_CHECK((std::is_same_v<traits::element_param_type, boost::call_traits<traits::element_type>::param_type>));
    }

    BOOST_AUTO_TEST_CASE(test_traits_bool_type)
    {
        using traits = database_data_type_traits<database_data_type::bool_type>;

        BOOST_CHECK((std::is_same_v<traits::type, bool>));
        BOOST_CHECK((std::is_same_v<traits::element_type, bool>));
        BOOST_CHECK((std::is_same_v<traits::param_type, boost::call_traits<traits::type>::param_type>));
        BOOST_CHECK((std::is_same_v<traits::element_param_type, boost::call_traits<traits::element_type>::param_type>));
    }

    BOOST_AUTO_TEST_CASE(test_traits_byte_array_type)
    {
        using traits = database_data_type_traits<database_data_type::byte_array_type>;

        BOOST_CHECK((std::is_same_v<traits::type, std::vector<uint8_t>>));
        BOOST_CHECK((std::is_same_v<traits::element_type, uint8_t>));
        BOOST_CHECK((std::is_same_v<traits::param_type, boost::call_traits<traits::type>::param_type>));
        BOOST_CHECK((std::is_same_v<traits::element_param_type, boost::call_traits<traits::element_type>::param_type>));
    }

    BOOST_AUTO_TEST_CASE(test_traits_document_type)
    {
        using traits = database_data_type_traits<database_data_type::document_type>;

        BOOST_CHECK((std::is_same_v<traits::type, std::vector<basis_database>>));
        BOOST_CHECK((std::is_same_v<traits::element_type, std::vector<basis_database>>));
        BOOST_CHECK((std::is_same_v<traits::param_type, boost::call_traits<traits::type>::param_type>));
        BOOST_CHECK((std::is_same_v<traits::element_param_type, boost::call_traits<traits::element_type>::param_type>));
    }

    BOOST_AUTO_TEST_CASE(test_traits_document_array_type)
    {
        using traits = database_data_type_traits<database_data_type::document_array_type>;

        BOOST_CHECK((std::is_same_v<traits::type, std::vector<std::vector<basis_database>>>));
        BOOST_CHECK((std::is_same_v<traits::element_type, std::vector<basis_database>>));
        BOOST_CHECK((std::is_same_v<traits::param_type, boost::call_traits<traits::type>::param_type>));
        BOOST_CHECK((std::is_same_v<traits::element_param_type, boost::call_traits<traits::element_type>::param_type>));
    }

    BOOST_AUTO_TEST_CASE(test_aliases_string_types)
    {
        BOOST_CHECK((std::is_same_v<traits::string_type, std::string>));
        BOOST_CHECK((std::is_same_v<traits::string_array_type, std::vector<std::string>>));
        BOOST_CHECK((std::is_same_v<traits::string_array_element_type, std::string>));
    }

    BOOST_AUTO_TEST_CASE(test_aliases_int32_types)
    {
        BOOST_CHECK((std::is_same_v<traits::int32_type, int32_t>));
        BOOST_CHECK((std::is_same_v<traits::int32_count_type, int32_t>));
        BOOST_CHECK((std::is_same_v<traits::int32_array_type, std::vector<int32_t>>));
        BOOST_CHECK((std::is_same_v<traits::int32_array_element_type, int32_t>));
    }

    BOOST_AUTO_TEST_CASE(test_aliases_int64_types)
    {
        BOOST_CHECK((std::is_same_v<traits::int64_type, int64_t>));
        BOOST_CHECK((std::is_same_v<traits::int64_count_type, int64_t>));
        BOOST_CHECK((std::is_same_v<traits::int64_array_type, std::vector<int64_t>>));
        BOOST_CHECK((std::is_same_v<traits::int64_array_element_type, int64_t>));
    }

    BOOST_AUTO_TEST_CASE(test_aliases_double_types)
    {
        BOOST_CHECK((std::is_same_v<traits::double_type, double>));
        BOOST_CHECK((std::is_same_v<traits::double_array_type, std::vector<double>>));
        BOOST_CHECK((std::is_same_v<traits::double_array_element_type, double>));
    }

    BOOST_AUTO_TEST_CASE(test_aliases_bool_type)
    {
        BOOST_CHECK((std::is_same_v<traits::bool_type, bool>));
    }

    BOOST_AUTO_TEST_CASE(test_aliases_byte_array_type)
    {
        BOOST_CHECK((std::is_same_v<traits::byte_array_type, std::vector<uint8_t>>));
        BOOST_CHECK((std::is_same_v<traits::byte_array_element_type, uint8_t>));
    }

    BOOST_AUTO_TEST_CASE(test_aliases_document_types)
    {
        BOOST_CHECK((std::is_same_v<traits::document_type, std::vector<basis_database>>));
        BOOST_CHECK((std::is_same_v<traits::document_array_type, std::vector<std::vector<basis_database>>>));
        BOOST_CHECK((std::is_same_v<traits::document_array_element_type, std::vector<basis_database>>));
    }

    BOOST_AUTO_TEST_CASE(test_param_types_string)
    {
        BOOST_CHECK((std::is_convertible_v<traits::param_type::string_type, std::string>));
        BOOST_CHECK((std::is_convertible_v<traits::param_type::string_array_type, std::vector<std::string>>));
        BOOST_CHECK((std::is_convertible_v<traits::param_type::string_array_element_type, std::string>));
    }

    BOOST_AUTO_TEST_CASE(test_param_types_int32)
    {
        BOOST_CHECK((std::is_convertible_v<traits::param_type::int32_type, int32_t>));
        BOOST_CHECK((std::is_convertible_v<traits::param_type::int32_count_type, int32_t>));
        BOOST_CHECK((std::is_convertible_v<traits::param_type::int32_array_type, std::vector<int32_t>>));
        BOOST_CHECK((std::is_convertible_v<traits::param_type::int32_array_element_type, int32_t>));
    }

    BOOST_AUTO_TEST_CASE(test_param_types_int64)
    {
        BOOST_CHECK((std::is_convertible_v<traits::param_type::int64_type, int64_t>));
        BOOST_CHECK((std::is_convertible_v<traits::param_type::int64_count_type, int64_t>));
        BOOST_CHECK((std::is_convertible_v<traits::param_type::int64_array_type, std::vector<int64_t>>));
        BOOST_CHECK((std::is_convertible_v<traits::param_type::int64_array_element_type, int64_t>));
    }

    BOOST_AUTO_TEST_CASE(test_param_types_double)
    {
        BOOST_CHECK((std::is_convertible_v<traits::param_type::double_type, double>));
        BOOST_CHECK((std::is_convertible_v<traits::param_type::double_array_type, std::vector<double>>));
        BOOST_CHECK((std::is_convertible_v<traits::param_type::double_array_element_type, double>));
    }

    BOOST_AUTO_TEST_CASE(test_param_types_bool)
    {
        BOOST_CHECK((std::is_convertible_v<traits::param_type::bool_type, bool>));
    }

    BOOST_AUTO_TEST_CASE(test_param_types_byte_array)
    {
        BOOST_CHECK((std::is_convertible_v<traits::param_type::byte_array_type, std::vector<uint8_t>>));
        BOOST_CHECK((std::is_convertible_v<traits::param_type::byte_array_element_type, uint8_t>));
    }

    BOOST_AUTO_TEST_CASE(test_param_types_document)
    {
        BOOST_CHECK((std::is_convertible_v<traits::param_type::document_type, std::vector<basis_database>>));
        BOOST_CHECK((std::is_convertible_v<traits::param_type::document_array_type, std::vector<std::vector<basis_database>>>));
        BOOST_CHECK((std::is_convertible_v<traits::param_type::document_array_element_type, std::vector<basis_database>>));
    }

BOOST_AUTO_TEST_SUITE_END()
