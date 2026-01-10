#include "json_value_to_basis_converter.h"

#include <boost/numeric/conversion/cast.hpp>

celeritas::basis_database celeritas::json_value_to_basis_converter::convert(std::string_view key, const json_value& value)
{
    const auto& converters = get_converters();
    if (const auto iter = converters.find(value.kind());
        iter != converters.end())
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

    const auto& first = array.front();
    if (first.is_string())
    {
        basis_database::string_array result{};
        result.reserve(array.size());
        for (const auto& element : array)
        {
            result.emplace_back(element.get_string());
        }
        return basis_database{ key, result };
    }

    if (first.is_int64())
    {
        basis_database::int64_array result{};
        result.reserve(array.size());
        for (const auto& element : array)
        {
            result.emplace_back(element.get_int64());
        }
        return basis_database{ key, result };
    }

    if (first.is_uint64())
    {
        basis_database::int64_array result{};
        result.reserve(array.size());
        for (const auto& element : array)
        {
            result.emplace_back(boost::numeric_cast<int64_t>(element.get_uint64()));
        }
        return basis_database{ key, result };
    }

    if (first.is_double())
    {
        basis_database::double_array result{};
        result.reserve(array.size());
        for (const auto& element : array)
        {
            result.emplace_back(element.get_double());
        }
        return basis_database{ key, result };
    }

    if (first.is_object())
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
