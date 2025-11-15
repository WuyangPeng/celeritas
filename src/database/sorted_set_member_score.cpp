#include "sorted_set_member_score.h"

celeritas::sorted_set_member_score::sorted_set_member_score(const std::string& member, double score)
    : member_{ member }, score_{ score }
{
}

std::string celeritas::sorted_set_member_score::get_member() const
{
    return member_;
}

double celeritas::sorted_set_member_score::get_score() const
{
    return score_;
}

