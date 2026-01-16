#pragma once

#include "database/document/test/logs_data.h"
#include "database/document/test/properties_data.h"

#include <boost/test/unit_test.hpp>

namespace celeritas
{
    template <typename T>
    void check_array(const T& lhs, const T& rhs)
    {
        BOOST_CHECK_EQUAL(lhs.size(), rhs.size());
        if (lhs.size() != rhs.size())
        {
            return;
        }

        for (auto i = 0; i < lhs.size(); ++i)
        {
            if constexpr (std::is_floating_point_v<typename T::value_type>)
            {
                BOOST_CHECK_CLOSE(lhs.at(i), rhs.at(i), 0.001);
            }
            else
            {
                BOOST_CHECK_EQUAL(lhs.at(i), rhs.at(i));
            }
        }
    }

    void check_properties_data(const traits::document_type& lhs, const traits::document_type& rhs);

    void check_properties_data(const properties_data& lhs, const properties_data& rhs);

    void check_logs_data(const traits::document_array_type& lhs, const traits::document_array_type& rhs);

    void check_logs_data(const logs_data& lhs, const logs_data& rhs);
}