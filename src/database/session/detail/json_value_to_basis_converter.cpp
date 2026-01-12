#include "json_value_to_basis_converter.tpp"
#include "common/core/celeritas_error.h"

#include <boost/numeric/conversion/cast.hpp>

celeritas::basis_database celeritas::json_value_to_basis_converter::convert(const std::string_view key, const json_value& value)
{
    const auto& converters = get_converters();
    if (const auto iter = converters.find(value.kind());
        iter != converters.cend())
    {
        return iter->second(key, value);
    }

    return convert_null(key, value);
}

const celeritas::json_value_to_basis_converter::container_type& celeritas::json_value_to_basis_converter::get_converters()
{
    static const container_type container{ { boost::json::kind::object, convert_object },
                                           { boost::json::kind::array, convert_array },
                                           { boost::json::kind::string, convert_string },
                                           { boost::json::kind::int64, convert_int64 },
                                           { boost::json::kind::uint64, convert_uint64 },
                                           { boost::json::kind::double_, convert_double },
                                           { boost::json::kind::bool_, convert_bool },
                                           { boost::json::kind::null, convert_null } };

    return container;
}

const celeritas::json_value_to_basis_converter::array_container_type& celeritas::json_value_to_basis_converter::get_array_converters()
{
    static const array_container_type container{ { boost::json::kind::string, convert_array_string },
                                                 { boost::json::kind::int64, convert_array_int64 },
                                                 { boost::json::kind::uint64, convert_array_uint64 },
                                                 { boost::json::kind::double_, convert_array_double },
                                                 { boost::json::kind::object, convert_array_object },
                                                 { boost::json::kind::array, convert_array_unsupported },
                                                 { boost::json::kind::null, convert_array_unsupported },
                                                 { boost::json::kind::bool_, convert_array_unsupported } };
    return container;
}

celeritas::basis_database celeritas::json_value_to_basis_converter::convert_object(const std::string_view key, const json_value& value)
{
    basis_database::document_type document{};
    const auto& object = value.get_object();
    document.reserve(object.size());
    for (const auto& element : object)
    {
        document.emplace_back(convert(element.key(), element.value()));
    }
    return basis_database{ key, document };
}

celeritas::basis_database celeritas::json_value_to_basis_converter::convert_array(const std::string_view key, const json_value& value)
{
    const auto& array = value.get_array();
    if (array.empty())
    {
        return basis_database{ key, basis_database::string_array{} };
    }

    const auto& converters = get_array_converters();
    if (const auto iter = converters.find(array.front().kind());
        iter != converters.cend())
    {
        return iter->second(key, array);
    }

    return basis_database{ key, basis_database::string_array{} };
}

celeritas::basis_database celeritas::json_value_to_basis_converter::convert_string(const std::string_view key, const json_value& value)
{
    return basis_database{ key, std::string{ value.get_string() } };
}

celeritas::basis_database celeritas::json_value_to_basis_converter::convert_int64(const std::string_view key, const json_value& value)
{
    return basis_database{ key, value.get_int64() };
}

celeritas::basis_database celeritas::json_value_to_basis_converter::convert_uint64(const std::string_view key, const json_value& value)
{
    return basis_database{ key, boost::numeric_cast<int64_t>(value.get_uint64()) };
}

celeritas::basis_database celeritas::json_value_to_basis_converter::convert_double(const std::string_view key, const json_value& value)
{
    return basis_database{ key, value.get_double() };
}

celeritas::basis_database celeritas::json_value_to_basis_converter::convert_bool(const std::string_view key, const json_value& value)
{
    return basis_database{ key, value.get_bool() };
}

celeritas::basis_database celeritas::json_value_to_basis_converter::convert_null(const std::string_view key, const json_value& value)
{
    return basis_database{ key };
}

celeritas::basis_database celeritas::json_value_to_basis_converter::convert_array_string(const std::string_view key, const boost::json::array& array)
{
    return convert_array_impl<basis_database::string_array>(key, array, [](const auto& element) {
        return std::string{ element.get_string() };
    });
}

celeritas::basis_database celeritas::json_value_to_basis_converter::convert_array_int64(const std::string_view key, const boost::json::array& array)
{
    return convert_array_impl<basis_database::int64_array>(key, array, [](const auto& element) {
        return element.get_int64();
    });
}

celeritas::basis_database celeritas::json_value_to_basis_converter::convert_array_uint64(const std::string_view key, const boost::json::array& array)
{
    return convert_array_impl<basis_database::int64_array>(key, array, [](const auto& element) {
        return boost::numeric_cast<int64_t>(element.get_uint64());
    });
}

celeritas::basis_database celeritas::json_value_to_basis_converter::convert_array_double(const std::string_view key, const boost::json::array& array)
{
    return convert_array_impl<basis_database::double_array>(key, array, [](const auto& element) {
        return element.get_double();
    });
}

celeritas::basis_database celeritas::json_value_to_basis_converter::convert_array_object(const std::string_view key, const boost::json::array& array)
{
    basis_database::document_array result{};
    result.reserve(array.size());
    for (const auto& element : array)
    {
        basis_database::document_type document{};
        const auto& object = element.get_object();
        document.reserve(object.size());
        for (const auto& key_value : object)
        {
            document.emplace_back(convert(key_value.key(), key_value.value()));
        }
        result.emplace_back(std::move(document));
    }
    return basis_database{ key, result };
}

celeritas::basis_database celeritas::json_value_to_basis_converter::convert_array_unsupported(const std::string_view key, const boost::json::array& array)
{
    throw celeritas_error{ "Unsupported type for json value to basis" };
}
