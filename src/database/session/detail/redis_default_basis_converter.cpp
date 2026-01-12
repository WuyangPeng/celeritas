#include "redis_default_basis_converter.h"
#include "database/basic/database_data_type.h"

celeritas::basis_database celeritas::redis_default_basis_converter::get_default_basis_database(const database_field& field_name)
{
    const auto& converters = get_converters();
    if (const auto iter = converters.find(field_name.get_data_type());
        iter != converters.cend())
    {
        return iter->second(field_name);
    }

    return get_default_basis(field_name);
}

const celeritas::redis_default_basis_converter::container_type& celeritas::redis_default_basis_converter::get_converters()
{
    static const container_type container{ { database_data_type::string_type, get_string_basis },
                                           { database_data_type::document_type, get_document_basis },
                                           { database_data_type::int32_type, get_int32_basis },
                                           { database_data_type::int32_count_type, get_int32_basis },
                                           { database_data_type::int64_type, get_int64_basis },
                                           { database_data_type::int64_count_type, get_int64_basis },
                                           { database_data_type::double_type, get_double_basis },
                                           { database_data_type::bool_type, get_bool_basis },
                                           { database_data_type::string_array_type, get_string_array_basis },
                                           { database_data_type::document_array_type, get_document_array_basis },
                                           { database_data_type::int32_array_type, get_int32_array_basis },
                                           { database_data_type::int64_array_type, get_int64_array_basis },
                                           { database_data_type::double_array_type, get_double_array_basis },
                                           { database_data_type::byte_array_type, get_byte_array_basis } };

    return container;
}

celeritas::basis_database celeritas::redis_default_basis_converter::get_string_basis(const database_field& field_name)
{
    return basis_database{ field_name.get_field_name(), std::string{} };
}

celeritas::basis_database celeritas::redis_default_basis_converter::get_document_basis(const database_field& field_name)
{
    return basis_database{ field_name.get_field_name(), basis_database::document_type{} };
}

celeritas::basis_database celeritas::redis_default_basis_converter::get_int32_basis(const database_field& field_name)
{
    return basis_database{ field_name.get_field_name(), 0 };
}

celeritas::basis_database celeritas::redis_default_basis_converter::get_int64_basis(const database_field& field_name)
{
    return basis_database{ field_name.get_field_name(), int64_t{} };
}

celeritas::basis_database celeritas::redis_default_basis_converter::get_double_basis(const database_field& field_name)
{
    return basis_database{ field_name.get_field_name(), 0.0 };
}

celeritas::basis_database celeritas::redis_default_basis_converter::get_bool_basis(const database_field& field_name)
{
    return basis_database{ field_name.get_field_name(), false };
}

celeritas::basis_database celeritas::redis_default_basis_converter::get_string_array_basis(const database_field& field_name)
{
    return basis_database{ field_name.get_field_name(), basis_database::string_array{} };
}

celeritas::basis_database celeritas::redis_default_basis_converter::get_document_array_basis(const database_field& field_name)
{
    return basis_database{ field_name.get_field_name(), basis_database::document_array{} };
}

celeritas::basis_database celeritas::redis_default_basis_converter::get_int32_array_basis(const database_field& field_name)
{
    return basis_database{ field_name.get_field_name(), basis_database::int32_array{} };
}

celeritas::basis_database celeritas::redis_default_basis_converter::get_int64_array_basis(const database_field& field_name)
{
    return basis_database{ field_name.get_field_name(), basis_database::int64_array{} };
}

celeritas::basis_database celeritas::redis_default_basis_converter::get_double_array_basis(const database_field& field_name)
{
    return basis_database{ field_name.get_field_name(), basis_database::double_array{} };
}

celeritas::basis_database celeritas::redis_default_basis_converter::get_byte_array_basis(const database_field& field_name)
{
    return basis_database{ field_name.get_field_name(), basis_database::byte_array{} };
}

celeritas::basis_database celeritas::redis_default_basis_converter::get_default_basis(const database_field& field_name)
{
    return basis_database{ field_name.get_field_name(), std::string{} };
}
