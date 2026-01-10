#pragma once

#include "database/basic/basis_database.h"

#include <bsoncxx/types.hpp>
#include <bsoncxx/array/view.hpp>

#include <functional>
#include <map>
#include <string_view>

namespace celeritas
{
    class mongo_array_to_basis_converter
    {
    public:
        using class_type = mongo_array_to_basis_converter;
        using array_type = bsoncxx::array::view;
        using get_basis_from_array_function = std::function<basis_database(std::string_view, const array_type&)>;
        using container_type = std::map<bsoncxx::type, get_basis_from_array_function>;

        [[nodiscard]] static const container_type& get_converters();

    private:
        using document_array = basis_database::document_array;

        [[nodiscard]] static basis_database get_double_array(std::string_view key, const array_type& array);

        [[nodiscard]] static basis_database get_string_array(std::string_view key, const array_type& array);

        [[nodiscard]] static basis_database get_document_array(std::string_view key, const array_type& array);

        [[nodiscard]] static basis_database get_int32_array(std::string_view key, const array_type& array);

        [[nodiscard]] static basis_database get_int64_array(std::string_view key, const array_type& array);

        template <typename T>
        [[nodiscard]] static auto get_array_from_view(const array_type& row_view_array);

        [[nodiscard]] static document_array get_document_array_from_view(const array_type& row_view_array);
    };
}
