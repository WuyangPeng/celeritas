#pragma once

#include <string>

namespace celeritas
{
    class sorted_set_member_score
    {
    public:
        using class_type = sorted_set_member_score;

        sorted_set_member_score(std::string member, double score);

        [[nodiscard]] std::string get_member() const;

        [[nodiscard]] double get_score() const;

    private:
        std::string member_;
        double score_ = 0.0;
    };
}