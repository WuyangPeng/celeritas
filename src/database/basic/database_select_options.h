#pragma once

#include "database_field.h"

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace celeritas
{
    enum class sort_order
    {
        asc,
        desc
    };

    struct database_order_by
    {
        std::string_view field_name;
        sort_order order = sort_order::asc;
    };

    // 表示一组字段上的 OR LIKE 条件：
    //   (`field1` LIKE '%pattern%' OR `field2` LIKE '%pattern%' ...)
    struct database_like_condition
    {
        std::vector<std::string_view> field_names; // 参与模糊匹配的字段列表（OR 关系）
        std::string pattern;                       // LIKE 的匹配值（不含 %，内部自动包裹）
    };

    class database_select_options
    {
    public:
        using class_type = database_select_options;
        using order_by_container = std::vector<database_order_by>;
        using database_field_container = std::vector<database_field>;
        using like_condition_container = std::vector<database_like_condition>;

        database_select_options& add_order_by(std::string_view field_name, sort_order order = sort_order::asc);

        database_select_options& set_limit(int64_t limit);

        database_select_options& set_offset(int64_t offset);

        // 添加一组 OR-LIKE 条件（各字段之间为 OR 关系，多组条件之间为 AND 关系）
        database_select_options& add_like_condition(std::vector<std::string_view> field_names, std::string pattern);

        [[nodiscard]] const order_by_container& get_order_by() const noexcept;

        [[nodiscard]] std::optional<int64_t> get_limit() const noexcept;

        [[nodiscard]] std::optional<int64_t> get_offset() const noexcept;

        [[nodiscard]] const like_condition_container& get_like_conditions() const noexcept;

        void validate(const database_field_container& field_name_container) const;

    private:
        order_by_container order_by_{};
        std::optional<int64_t> limit_{};
        std::optional<int64_t> offset_{};
        like_condition_container like_conditions_{};
    };
}
