#pragma once

#include "mysql_statement_generator.h"

#include <sstream>

template <typename C, typename F>
void celeritas::mysql_statement_generator::join_container(std::stringstream& ss, const C& container, const std::string_view separator, const std::string_view tail_separator, F&& func)
{
    for (auto iter = container.begin(); iter != container.end(); ++iter)
    {
        func(*iter);

        if (std::next(iter) != container.end())
        {
            ss << separator;
        }
        else if (!tail_separator.empty())
        {
            ss << tail_separator;
        }
    }
}