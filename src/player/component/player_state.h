#pragma once

#include "player_component.h"
#include "player_manager.h"
#include "common/common_fwd.h"
#include "database/generated/mysql/player/user.h"
#include "message/basic/header.h"
#include "player/component/player_component_type.h"
#include "proto/service/player.pb.h"

#include <boost/asio/awaitable.hpp>
#include <boost/asio/io_context.hpp>

#include <array>
#include <memory>

namespace celeritas
{
    class player_state
    {
    public:
        using class_type = player_state;
        using player_component_shared_ptr = std::shared_ptr<player_component>;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using resource_loader_shared_ptr = std::shared_ptr<resource_loader_base>;
        using protobuf_message = google::protobuf::Message;
        using io_context_type = boost::asio::io_context;
        using any_io_executor = boost::asio::any_io_executor;
        using service_login_request_type = proto::service::service_login_request;

        player_state(const user& user,
                     const resource_loader_shared_ptr& resource_loader,
                     const any_io_executor& any_io_executor,
                     std::string instance_id,
                     const service_login_request_type& login);

        virtual ~player_state() noexcept = default;

        player_state(const player_state& rhs) = default;

        player_state& operator=(const player_state& rhs) = delete;

        player_state(player_state&& rhs) noexcept = default;

        player_state& operator=(player_state&& rhs) noexcept = delete;

        template <typename T>
        [[nodiscard]] std::shared_ptr<T> get_component() const;

        [[nodiscard]] player_component_shared_ptr get_component(player_component_type player_component_type) const;

        // 数据库数据加载
        [[nodiscard]] void_awaitable_type on_load_db();

        // 数据库数据解析
        [[nodiscard]] void_awaitable_type on_db_analysis();

        // 安全地访问其他组件的数据，解决组件间的依赖关系。
        [[nodiscard]] void_awaitable_type on_dependencies_ready();

        // 执行所有的初始同步消息发送。
        [[nodiscard]] void_awaitable_type send_initial_sync();

        // 所有数据稳定后，执行最终启动逻辑。
        [[nodiscard]] void_awaitable_type on_login();

        // 玩家登出
        [[nodiscard]] void_awaitable_type on_logout();

        // 保存数据库
        [[nodiscard]] void_awaitable_type save_db();

        void set_dirty();

        void set_player_state_type(player_state_type player_state_type);

        [[nodiscard]] player_state_type get_player_state_type() const;

        [[nodiscard]] int64_t get_user_id() const noexcept;

        [[nodiscard]] std::string get_game_server_id() const;

        [[nodiscard]] static std::string generate_token();

        [[nodiscard]] bool write(const std::string& server_type, const std::string& instance_id, const header& header, const protobuf_message& request);

        [[nodiscard]] std::string get_instance_id() const;

        void set_instance_id(const std::string& instance_id);

        [[nodiscard]] void_awaitable_type time_callback(const player_time_refresh_key& player_time_refresh_key, bool is_login) const;

        void set_mock_player_component(const player_component_shared_ptr& mock);

        void set_login(const service_login_request_type& login);

        [[nodiscard]] any_io_executor get_any_io_executor();

    private:
        using component_container_type = std::array<player_component_shared_ptr, static_cast<int>(player_component_type::max_component)>;
        using resource_loader_weak_ptr = std::weak_ptr<resource_loader_base>;

        void check() const;

        bool dirty_;
        player_state_type player_state_;
        component_container_type components_;
        resource_loader_weak_ptr resource_loader_;
        std::string instance_id_;
        any_io_executor strand_;
    };
}