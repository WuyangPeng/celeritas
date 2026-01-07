// 此文件是自动生成，请勿手动修改。

#pragma once

#include "database/basic/database_data_type_traits.h"
#include "database/basic/database_index_type.h"
#include "database/entity/database_entity.h"
#include "database/entity/entity.h"

namespace celeritas
{
    class server_cell final : public database_entity
    {
    public:
        using class_type = server_cell;
        using base_type = database_entity;

        explicit server_cell(const database_entity_change& entity);

        server_cell(database_type database_type, const database_entity_change& entity);

        server_cell(database_type database_type, traits::param_type::int64_type cell_id);

        [[nodiscard]] traits::int64_type get_cell_id() const noexcept;

        [[nodiscard]] traits::string_type get_game_server_id() const;

        [[nodiscard]] traits::string_type get_server_name() const;

        [[nodiscard]] traits::int64_type get_app_id() const noexcept;

        [[nodiscard]] traits::int64_type get_launch_time() const noexcept;

        [[nodiscard]] traits::string_type get_zone() const;

        [[nodiscard]] traits::bool_type is_is_close_display() const noexcept;

        [[nodiscard]] traits::int32_type get_status() const noexcept;

        void set_cell_id(traits::param_type::int64_type cell_id);

        void set_game_server_id(traits::param_type::string_type game_server_id);

        void set_server_name(traits::param_type::string_type server_name);

        void set_app_id(traits::param_type::int64_type app_id);

        void set_launch_time(traits::param_type::int64_type launch_time);

        void set_zone(traits::param_type::string_type zone);

        void set_is_close_display(traits::param_type::bool_type is_close_display);

        void set_status(traits::param_type::int32_type status);

        [[nodiscard]] static const database_field_container& get_database_field_container();

        [[nodiscard]] static const_database_entity_change_shared_ptr get_select(database_type database_type);

        [[nodiscard]] static const_database_entity_change_shared_ptr get_select(database_type database_type, traits::param_type::int64_type cell_id);

        [[nodiscard]] static const_database_entity_change_shared_ptr get_select(database_type database_type, const const_basis_database_container_shared_ptr& key);

        static constexpr std::string_view database_name{ "server_cell" };

        static constexpr std::string_view cell_id_describe{ "cell_id" };
        static constexpr std::string_view game_server_id_describe{ "game_server_id" };
        static constexpr std::string_view server_name_describe{ "server_name" };
        static constexpr std::string_view app_id_describe{ "app_id" };
        static constexpr std::string_view launch_time_describe{ "launch_time" };
        static constexpr std::string_view zone_describe{ "zone" };
        static constexpr std::string_view is_close_display_describe{ "is_close_display" };
        static constexpr std::string_view status_describe{ "status" };

    private:
        [[nodiscard]] static const_basis_database_container_shared_ptr get_key_basis_database_container(traits::param_type::int64_type cell_id);

        entity<cell_id_describe, database_data_type::int64_type, database_index_type::key> cell_id_;
        entity<game_server_id_describe, database_data_type::string_type, database_index_type::unique_index> game_server_id_;
        entity<server_name_describe, database_data_type::string_type> server_name_;
        entity<app_id_describe, database_data_type::int64_type, database_index_type::index> app_id_;
        entity<launch_time_describe, database_data_type::int64_type> launch_time_;
        entity<zone_describe, database_data_type::string_type> zone_;
        entity<is_close_display_describe, database_data_type::bool_type> is_close_display_;
        entity<status_describe, database_data_type::int32_type> status_;
    };
}