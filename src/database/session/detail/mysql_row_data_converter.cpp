#include "json_value_to_basis_converter.h"
#include "mysql_row_data_converter.h"
#include "mysql_row_data_converter.tpp"
#include "common/logging/logger.h"
#include "database/basic/basis_database.tpp"
#include "database/basic/database_data_type.h"
#include "database/basic/database_field.h"

celeritas::basis_database celeritas::mysql_row_data_converter::get_basis_database(const database_field& field_name, const field_view_type& row_view)
{
    const auto& converters = get_converters();
    if (const auto iter = converters.find(field_name.get_data_type());
        iter != converters.cend())
    {
        return iter->second(field_name, row_view);
    }

    return convert_default(field_name, row_view);
}

const celeritas::mysql_row_data_converter::container_type& celeritas::mysql_row_data_converter::get_converters()
{
    static const container_type container{ { database_data_type::string_type, convert_string },
                                           { database_data_type::document_type, convert_document },
                                           { database_data_type::int32_type, convert_int32 },
                                           { database_data_type::int32_count_type, convert_int32 },
                                           { database_data_type::int64_type, convert_int64 },
                                           { database_data_type::int64_count_type, convert_int64 },
                                           { database_data_type::double_type, convert_double },
                                           { database_data_type::bool_type, convert_bool },
                                           { database_data_type::string_array_type, convert_string_array },
                                           { database_data_type::document_array_type, convert_document_array },
                                           { database_data_type::int32_array_type, convert_int32_array },
                                           { database_data_type::int64_array_type, convert_int64_array },
                                           { database_data_type::double_array_type, convert_double_array },
                                           { database_data_type::byte_array_type, convert_byte_array } };

    return container;
}

celeritas::basis_database celeritas::mysql_row_data_converter::convert_string(const database_field& field_name, const field_view_type& row_view)
{
    return basis_database{ field_name.get_field_name(), row_view.as_string() };
}

celeritas::basis_database celeritas::mysql_row_data_converter::convert_document(const database_field& field_name, const field_view_type& row_view)
{
    const std::string value{ row_view.as_string() };
    if (value.empty())
    {
        return basis_database{ field_name.get_field_name(), basis_database::document_type{} };
    }

    return do_convert_document(field_name, value);
}

celeritas::basis_database celeritas::mysql_row_data_converter::do_convert_document(const database_field& field_name, const std::string& value)
{
    auto json_value = boost::json::parse(value);

    basis_database::document_type document{};
    const auto& object = json_value.get_object();
    document.reserve(object.size());
    for (const auto& element : object)
    {
        document.emplace_back(json_value_to_basis_converter::convert(element.key(), element.value()));
    }
    return basis_database{ field_name.get_field_name(), document };
}

celeritas::basis_database celeritas::mysql_row_data_converter::convert_int32(const database_field& field_name, const field_view_type& row_view)
{
    return basis_database{ field_name.get_field_name(), boost::numeric_cast<int32_t>(row_view.as_int64()) };
}

celeritas::basis_database celeritas::mysql_row_data_converter::convert_int64(const database_field& field_name, const field_view_type& row_view)
{
    return basis_database{ field_name.get_field_name(), row_view.as_int64() };
}

celeritas::basis_database celeritas::mysql_row_data_converter::convert_double(const database_field& field_name, const field_view_type& row_view)
{
    return basis_database{ field_name.get_field_name(), row_view.as_double() };
}

celeritas::basis_database celeritas::mysql_row_data_converter::convert_bool(const database_field& field_name, const field_view_type& row_view)
{
    return basis_database{ field_name.get_field_name(), row_view.as_int64() != 0 };
}

celeritas::basis_database celeritas::mysql_row_data_converter::convert_string_array(const database_field& field_name, const field_view_type& row_view)
{
    const std::string value{ row_view.as_string() };
    if (value.empty())
    {
        return basis_database{ field_name.get_field_name(), basis_database::string_array{} };
    }

    const auto json = boost::json::parse(value);
    const auto result = boost::json::value_to<basis_database::string_array>(json);

    return basis_database{ field_name.get_field_name(), result };
}

celeritas::basis_database celeritas::mysql_row_data_converter::convert_document_array(const database_field& field_name, const field_view_type& row_view)
{
    const std::string value{ row_view.as_string() };
    if (value.empty())
    {
        return basis_database{ field_name.get_field_name(), basis_database::document_array{} };
    }

    return do_convert_document_array(field_name, value);
}

celeritas::basis_database celeritas::mysql_row_data_converter::do_convert_document_array(const database_field& field_name, const std::string& value)
{
    auto json_value = boost::json::parse(value);

    basis_database::document_array result{};

    const auto& array = json_value.get_array();
    result.reserve(array.size());
    for (const auto& element : array)
    {
        basis_database::document_type document{};
        const auto& object = element.get_object();
        document.reserve(object.size());
        for (const auto& key_value : object)
        {
            document.emplace_back(json_value_to_basis_converter::convert(key_value.key(), key_value.value()));
        }
        result.emplace_back(std::move(document));
    }

    return basis_database{ field_name.get_field_name(), result };
}

celeritas::basis_database celeritas::mysql_row_data_converter::convert_int32_array(const database_field& field_name, const field_view_type& row_view)
{
    return to_numeric_array_basis<basis_database::int32_array>(field_name, row_view);
}

celeritas::basis_database celeritas::mysql_row_data_converter::convert_int64_array(const database_field& field_name, const field_view_type& row_view)
{
    return to_numeric_array_basis<basis_database::int64_array>(field_name, row_view);
}

celeritas::basis_database celeritas::mysql_row_data_converter::convert_double_array(const database_field& field_name, const field_view_type& row_view)
{
    return to_numeric_array_basis<basis_database::double_array>(field_name, row_view);
}

celeritas::basis_database celeritas::mysql_row_data_converter::convert_byte_array(const database_field& field_name, const field_view_type& row_view)
{
    const std::string value{ row_view.as_string() };

    const basis_database::byte_array result{ value.cbegin(), value.cend() };

    return basis_database{ field_name.get_field_name(), result };
}

celeritas::basis_database celeritas::mysql_row_data_converter::convert_default(const database_field& field_name, const field_view_type& row_view)
{
    return basis_database{ field_name.get_field_name(), std::string{} };
}
