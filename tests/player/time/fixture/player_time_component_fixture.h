#pragma once

#include "helper/io_context_fixture.h"
#include "player/mock/mock_database_pool.h"
#include "player/mock/mock_player_component.h"
#include "player/mock/mock_player_state.h"
#include "player/time/player_time_component.h"

#include <memory>

namespace celeritas
{
    class player_time_component_fixture : public io_context_fixture
    {
    public:
        using class_type = player_time_component_fixture;
        using base_type = io_context_fixture;
        using mock_database_pool_shared_ptr = std::shared_ptr<mock_database_pool>;
        using player_time_component_shared_ptr = std::shared_ptr<player_time_component>;
        using mock_player_component_shared_ptr = std::shared_ptr<mock_player_component>;

        player_time_component_fixture();

        ~player_time_component_fixture() override;

        void run_io_context_two_times();

        [[nodiscard]] mock_database_pool_shared_ptr get_mock_pool();

        [[nodiscard]] player_time_component_shared_ptr get_component();

        [[nodiscard]] mock_player_component_shared_ptr get_mock_player_component();

        [[nodiscard]] mock_player_state& get_mock_player_state();

    private:
        mock_player_state mock_player_state_;
        mock_database_pool_shared_ptr mock_pool_;
        player_time_component_shared_ptr component_;
        mock_player_component_shared_ptr mock_player_component_;
    };
}
