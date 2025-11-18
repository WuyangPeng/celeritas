#include "mongo_parameter.h"

celeritas::mongo_parameter::mongo_parameter(std::string_view uri, std::string_view db_name)
    : uri_{ uri }, db_name_{ db_name }
{
}

std::string celeritas::mongo_parameter::get_uri() const
{
    return uri_;
}

std::string celeritas::mongo_parameter::get_db_name() const
{
    return db_name_;
}
