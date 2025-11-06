#include "vertex_hash.h"

#include <boost/container_hash/hash.hpp>

size_t celeritas::vertex_hash::operator()(const vertex& u) const
{
    std::size_t seed = 0;

    boost::hash_combine(seed, u[0]);
    boost::hash_combine(seed, u[1]);

    return seed;
}