#include "database/basic/basis_database.h"
#include "database/basic/database_data_type_traits.h"

#include <boost/test/unit_test.hpp>

#include <cstdint>
#include <string>
#include <type_traits>
#include <vector>

using namespace celeritas;

BOOST_AUTO_TEST_SUITE(database_data_type_traits_suite)

    BOOST_AUTO_TEST_CASE(test_traits_types)
    {
        BOOST_CHECK((std::is_same_v<database_data_type_traits<database_data_type::null_type>::type, decltype(nullptr)>));
        BOOST_CHECK((std::is_same_v<database_data_type_traits<database_data_type::null_type>::element_type, decltype(nullptr)>));

        BOOST_CHECK((std::is_same_v<database_data_type_traits<database_data_type::string_type>::type, std::string>));
        BOOST_CHECK((std::is_same_v<database_data_type_traits<database_data_type::string_type>::element_type, std::string>));

        BOOST_CHECK((std::is_same_v<database_data_type_traits<database_data_type::string_array_type>::type, std::vector<std::string>>));
        BOOST_CHECK((std::is_same_v<database_data_type_traits<database_data_type::string_array_type>::element_type, std::string>));

        BOOST_CHECK((std::is_same_v<database_data_type_traits<database_data_type::int32_type>::type, int32_t>));
        BOOST_CHECK((std::is_same_v<database_data_type_traits<database_data_type::int32_type>::element_type, int32_t>));

        BOOST_CHECK((std::is_same_v<database_data_type_traits<database_data_type::int32_count_type>::type, int32_t>));
        BOOST_CHECK((std::is_same_v<database_data_type_traits<database_data_type::int32_count_type>::element_type, int32_t>));

        BOOST_CHECK((std::is_same_v<database_data_type_traits<database_data_type::int32_array_type>::type, std::vector<int32_t>>));
        BOOST_CHECK((std::is_same_v<database_data_type_traits<database_data_type::int32_array_type>::element_type, int32_t>));

        BOOST_CHECK((std::is_same_v<database_data_type_traits<database_data_type::int64_type>::type, int64_t>));
        BOOST_CHECK((std::is_same_v<database_data_type_traits<database_data_type::int64_type>::element_type, int64_t>));

        BOOST_CHECK((std::is_same_v<database_data_type_traits<database_data_type::int64_count_type>::type, int64_t>));
        BOOST_CHECK((std::is_same_v<database_data_type_traits<database_data_type::int64_count_type>::element_type, int64_t>));

        BOOST_CHECK((std::is_same_v<database_data_type_traits<database_data_type::int64_array_type>::type, std::vector<int64_t>>));
        BOOST_CHECK((std::is_same_v<database_data_type_traits<database_data_type::int64_array_type>::element_type, int64_t>));

        BOOST_CHECK((std::is_same_v<database_data_type_traits<database_data_type::double_type>::type, double>));
        BOOST_CHECK((std::is_same_v<database_data_type_traits<database_data_type::double_type>::element_type, double>));

        BOOST_CHECK((std::is_same_v<database_data_type_traits<database_data_type::double_array_type>::type, std::vector<double>>));
        BOOST_CHECK((std::is_same_v<database_data_type_traits<database_data_type::double_array_type>::element_type, double>));

        BOOST_CHECK((std::is_same_v<database_data_type_traits<database_data_type::bool_type>::type, bool>));
        BOOST_CHECK((std::is_same_v<database_data_type_traits<database_data_type::bool_type>::element_type, bool>));

        BOOST_CHECK((std::is_same_v<database_data_type_traits<database_data_type::byte_array_type>::type, std::vector<uint8_t>>));
        BOOST_CHECK((std::is_same_v<database_data_type_traits<database_data_type::byte_array_type>::element_type, uint8_t>));

        BOOST_CHECK((std::is_same_v<database_data_type_traits<database_data_type::document_type>::type, std::vector<basis_database>>));
        BOOST_CHECK((std::is_same_v<database_data_type_traits<database_data_type::document_type>::element_type, std::vector<basis_database>>));

        BOOST_CHECK((std::is_same_v<database_data_type_traits<database_data_type::document_array_type>::type, std::vector<std::vector<basis_database>>>));
        BOOST_CHECK((std::is_same_v<database_data_type_traits<database_data_type::document_array_type>::element_type, std::vector<basis_database>>));
    }

    BOOST_AUTO_TEST_CASE(test_traits_aliases)
    {
        BOOST_CHECK((std::is_same_v<traits::string_type, std::string>));
        BOOST_CHECK((std::is_same_v<traits::string_array_type, std::vector<std::string>>));
        BOOST_CHECK((std::is_same_v<traits::string_array_element_type, std::string>));

        BOOST_CHECK((std::is_same_v<traits::int32_type, int32_t>));
        BOOST_CHECK((std::is_same_v<traits::int32_count_type, int32_t>));
        BOOST_CHECK((std::is_same_v<traits::int32_array_type, std::vector<int32_t>>));
        BOOST_CHECK((std::is_same_v<traits::int32_array_element_type, int32_t>));

        BOOST_CHECK((std::is_same_v<traits::int64_type, int64_t>));
        BOOST_CHECK((std::is_same_v<traits::int64_count_type, int64_t>));
        BOOST_CHECK((std::is_same_v<traits::int64_array_type, std::vector<int64_t>>));
        BOOST_CHECK((std::is_same_v<traits::int64_array_element_type, int64_t>));

        BOOST_CHECK((std::is_same_v<traits::double_type, double>));
        BOOST_CHECK((std::is_same_v<traits::double_array_type, std::vector<double>>));
        BOOST_CHECK((std::is_same_v<traits::double_array_element_type, double>));

        BOOST_CHECK((std::is_same_v<traits::bool_type, bool>));

        BOOST_CHECK((std::is_same_v<traits::byte_array_type, std::vector<uint8_t>>));
        BOOST_CHECK((std::is_same_v<traits::byte_array_element_type, uint8_t>));

        BOOST_CHECK((std::is_same_v<traits::document_type, std::vector<basis_database>>));
        BOOST_CHECK((std::is_same_v<traits::document_array_type, std::vector<std::vector<basis_database>>>));
        BOOST_CHECK((std::is_same_v<traits::document_array_element_type, std::vector<basis_database>>));
    }

    BOOST_AUTO_TEST_CASE(test_traits_param_types)
    {
        BOOST_CHECK((std::is_convertible_v<traits::param_type::string_type, std::string>));
        BOOST_CHECK((std::is_convertible_v<traits::param_type::string_array_type, std::vector<std::string>>));
        BOOST_CHECK((std::is_convertible_v<traits::param_type::string_array_element_type, std::string>));

        BOOST_CHECK((std::is_convertible_v<traits::param_type::int32_type, int32_t>));
        BOOST_CHECK((std::is_convertible_v<traits::param_type::int32_count_type, int32_t>));
        BOOST_CHECK((std::is_convertible_v<traits::param_type::int32_array_type, std::vector<int32_t>>));
        BOOST_CHECK((std::is_convertible_v<traits::param_type::int32_array_element_type, int32_t>));

        BOOST_CHECK((std::is_convertible_v<traits::param_type::int64_type, int64_t>));
        BOOST_CHECK((std::is_convertible_v<traits::param_type::int64_count_type, int64_t>));
        BOOST_CHECK((std::is_convertible_v<traits::param_type::int64_array_type, std::vector<int64_t>>));
        BOOST_CHECK((std::is_convertible_v<traits::param_type::int64_array_element_type, int64_t>));

        BOOST_CHECK((std::is_convertible_v<traits::param_type::double_type, double>));
        BOOST_CHECK((std::is_convertible_v<traits::param_type::double_array_type, std::vector<double>>));
        BOOST_CHECK((std::is_convertible_v<traits::param_type::double_array_element_type, double>));
        BOOST_CHECK((std::is_convertible_v<traits::param_type::bool_type, bool>));

        BOOST_CHECK((std::is_convertible_v<traits::param_type::byte_array_type, std::vector<uint8_t>>));
        BOOST_CHECK((std::is_convertible_v<traits::param_type::byte_array_element_type, uint8_t>));

        BOOST_CHECK((std::is_convertible_v<traits::param_type::document_type, std::vector<basis_database>>));
        BOOST_CHECK((std::is_convertible_v<traits::param_type::document_array_type, std::vector<std::vector<basis_database>>>));
        BOOST_CHECK((std::is_convertible_v<traits::param_type::document_array_element_type, std::vector<basis_database>>));
    }

BOOST_AUTO_TEST_SUITE_END()
