#include "mongo_parameter.h"

celeritas::mongo_parameter::mongo_parameter(std::string uri, std::string db_name)
    : uri_{ std::move(uri) }, db_name_{ std::move(db_name) }
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
