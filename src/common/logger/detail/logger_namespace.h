#pragma once

#include <boost/log/trivial.hpp>

namespace celeritas
{
    namespace log_sinks = boost::log::sinks;
    namespace log_sources = boost::log::sources;
    namespace log_trivial = boost::log::trivial;
    namespace log_keywords = boost::log::keywords;
    namespace log_attributes = boost::log::attributes;
    namespace log_expressions = boost::log::expressions;

    using severity_level_type = log_trivial::severity_level;
}