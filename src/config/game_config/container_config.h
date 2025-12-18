#pragma once

#include <map>
#include <memory>
#include <optional>

namespace celeritas
{
    template <typename Element, typename IndexType = int>
    class container_config
    {
    public:
        using class_type = container_config;
        using const_config_shared_ptr = std::shared_ptr<const Element>;
        using optional_config = std::optional<const_config_shared_ptr>;
        using container = std::map<IndexType, const_config_shared_ptr>;

        [[nodiscard]] optional_config get(IndexType id) const;

        void add_config(const const_config_shared_ptr& config);

        [[nodiscard]] const container& get_container() const;

    private:
        container container_;
    };
}