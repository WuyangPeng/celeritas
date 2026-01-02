#pragma once

#include <cstdint>
#include <vector>

namespace celeritas
{
    class sequence_increment_fixture
    {
    public:
        using class_type = sequence_increment_fixture;
        using id_type = std::vector<int64_t>;

        static constexpr auto num_ids = 10;

        void init();

        void check_sequence_monotonicity();

    private:
        void do_check_sequence_monotonicity(int index);

        id_type ids_;
    };
}