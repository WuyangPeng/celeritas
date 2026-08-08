// 创建时间：2026-07-28
// 修改时间：2026-07-28
// 审核时间：2026-07-28

#include "admin_service_base.h"
#include "admin/admin_constant.h"
#include "database/database_constant.h"
#include "database/generated/mysql/admin/admin_account.h"
#include "database/generated/redis/admin/admin_session_token.h"
#include "database/pool/database_pool_base.h"
#include "database/pool/database_pool_manager.h"

celeritas::admin_service_base::optional_admin_account_awaitable celeritas::admin_service_base::get_admin_account(const database_pool_shared_ptr& mysql_pool,
                                                                                                                 const int64_t target_id)
{
    const auto optional_admin_account = co_await mysql_pool->select_one(admin_account::get_select(database_type::mysql, target_id),
                                                                        admin_account::get_database_field_container());

    if (optional_admin_account)
    {
        co_return admin_account{ *optional_admin_account };
    }

    co_return std::nullopt;
}

celeritas::admin_service_base::optional_int64_awaitable celeritas::admin_service_base::resolve_operator_id(const std::string& token)
{
    const auto redis_pool = database_pool_manager::get_instance().get_pool(redis_db_name.data());

    const auto optional_session_token = co_await redis_pool->select_one(admin_session_token::get_select(database_type::redis, token),
                                                                        admin_session_token::get_database_field_container());

    if (optional_session_token)
    {
        co_return admin_session_token{ *optional_session_token }.get_account_id();
    }

    co_return std::nullopt;
}
