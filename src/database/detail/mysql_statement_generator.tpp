#pragma once

#include "mysql_statement_generator.h"

template <typename C, typename F>
std::string celeritas::mysql_statement_generator::join_container(const C& container, const std::string_view separator, const std::string_view tail_separator, F&& func)
{
    std::string result{};

    for (auto iter = container.begin(); iter != container.end(); ++iter)
    {
        result += func(*iter);

        if (std::next(iter) != container.end())
        {
            result += separator;
        }
        else if (!tail_separator.empty())
        {
            result += tail_separator;
        }
    }

    return result;
}