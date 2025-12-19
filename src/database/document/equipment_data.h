#pragma once

namespace celeritas
{
    class equipment_data
    {
    public:
        using class_type = equipment_data;

        equipment_data();

        explicit equipment_data(int strength, int durability);

        [[nodiscard]] int get_strength() const;

        void set_strength(int strength);

        [[nodiscard]] int get_durability() const;

        void set_durability(int durability);

    private:
        int strength_;
        int durability_;
    };
}
