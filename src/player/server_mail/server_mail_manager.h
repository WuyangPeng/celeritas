#pragma once

#include "database/generated/mongo/admin/server_mail.h"

#include <boost/asio/awaitable.hpp>

#include <memory>
#include <optional>
#include <shared_mutex>
#include <unordered_map>
#include <vector>

namespace celeritas
{
    class server_mail_manager
    {
    public:
        using class_type = server_mail_manager;
        using server_mail_shared_ptr = std::shared_ptr<server_mail>;
        using const_server_mail_shared_ptr = std::shared_ptr<const server_mail>;
        using optional_const_server_mail_shared_ptr = std::optional<const_server_mail_shared_ptr>;
        using server_mail_container = std::vector<const_server_mail_shared_ptr>;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using database_pool_shared_ptr = std::shared_ptr<database_pool_base>;
        using any_io_executor = boost::asio::any_io_executor;

        [[nodiscard]] static server_mail_manager& get_instance();

        // 加载所有服务器邮件
        void load_all_mails(const any_io_executor& any_io_executor);

        // 根据id获取邮件
        [[nodiscard]] optional_const_server_mail_shared_ptr get_mail(int64_t mail_id);

        // 根据类型获取邮件列表
        [[nodiscard]] server_mail_container get_mails_by_type(int32_t mail_type);

        // 获取所有未过期的邮件
        [[nodiscard]] server_mail_container get_active_mails();

        // 获取所有邮件
        [[nodiscard]] server_mail_container get_all_mails();

        // 获取比某个id大且有效的所有邮件
        [[nodiscard]] server_mail_container get_mails_greater_than_id(int64_t mail_id);

        // 添加邮件
        void add_mail(const const_server_mail_shared_ptr& mail);

        // 根据id删除邮件
        void remove_mail(int64_t mail_id);

        // 清理过期邮件
        void clean_expired_mails(int64_t current_time);

        // 清空缓存
        void clear();

    private:
        using container_type = std::unordered_map<int64_t, const_server_mail_shared_ptr>;

        server_mail_manager();

        // 检查邮件是否过期
        [[nodiscard]] static bool is_expired(const server_mail& mail, int64_t current_time);

        [[nodiscard]] void_awaitable_type load_all_mails();

        container_type mails_;
        std::shared_mutex mutex_;
    };
}