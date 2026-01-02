#pragma once

#include <thread>
#include <unordered_set>
#include <vector>

namespace celeritas
{
    class uniqueness_fixture
    {
    public:
        using class_type = uniqueness_fixture;

        static constexpr auto num_threads = 10;
        static constexpr auto ids_per_thread = 1000;

        void init();

        [[nodiscard]] int get_generated_ids_size() const;

    private:
        void generate_ids_task(int index);

        std::vector<std::thread> threads_;
        std::unordered_set<int64_t> generated_ids_;
        std::mutex mutex_;
    };
}