#pragma once

#include <boost/test/unit_test.hpp>

#include <type_traits>

namespace celeritas
{
    template <typename ArrayType>
    void verify_array_values(const ArrayType& lhs, const ArrayType& rhs)
    {
        BOOST_CHECK_EQUAL(lhs.size(), rhs.size());
        if (lhs.size() != rhs.size())
        {
            return;
        }

        for (auto i = 0; i < lhs.size(); ++i)
        {
            if constexpr (std::is_floating_point_v<typename ArrayType::value_type>)
            {
                BOOST_CHECK_CLOSE(lhs.at(i), rhs.at(i), 0.001);
            }
            else
            {
                BOOST_CHECK_EQUAL(lhs.at(i), rhs.at(i));
            }
        }
    }
}