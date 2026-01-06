#include "scan_result.h"

celeritas::scan_result::scan_result(std::string cursor, array_type keys)
    : cursor_{ std::move(cursor) }, keys_{ std::move(keys) }
{
}

std::string celeritas::scan_result::get_cursor() const
{
    return cursor_;
}

celeritas::scan_result::array_type celeritas::scan_result::get_keys() const
{
    return keys_;
}