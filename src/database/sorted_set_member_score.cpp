#include "sorted_set_member_score.h"
#include "boost/phoenix/operator/member.hpp"

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

void celeritas::sorted_set_member_score::set_member(const std::string& member)
{
    member_ = member;
}

void celeritas::sorted_set_member_score::set_score(const double score)
{
    score_ = score;
}

