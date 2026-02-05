#pragma once

#include "database/generated/mysql/player/user.h"
#include "helper/io_context_fixture.h"
#include "player/mock/mock_database_pool.h"
#include "player/mock/mock_player_state.h"

#include <memory>

namespace celeritas
{
    class player_user_component_fixture : public io_context_fixture
    {
    public:
        using class_type = player_user_component_fixture;
        using base_type = io_context_fixture;
        using mock_database_pool_shared_ptr = std::shared_ptr<mock_database_pool>;

        player_user_component_fixture();

        ~player_user_component_fixture() override;

        [[nodiscard]] mock_database_pool_shared_ptr get_mock_pool();

        [[nodiscard]] user& get_test_user();

        [[nodiscard]] mock_player_state& get_mock_player_state();

    private:
        mock_database_pool_shared_ptr mock_pool_;
        user test_user_;
        mock_player_state mock_player_state_;
    };
}
