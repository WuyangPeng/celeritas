#pragma once

#include <vector>

namespace celeritas
{
    class weight
    {
    public:
        using class_type = weight;
        using id_container_type = std::vector<int>;
        using weights_type = std::vector<double>;

        void clear() noexcept;

        void add_element(int id, double weights);

        [[nodiscard]] int get_random_index_by_weight() const;

        [[nodiscard]] int get_id(int index) const;

        [[nodiscard]] int get_random_id_by_weight() const;

    private:
        id_container_type id_;
        weights_type weights_;
    };
}