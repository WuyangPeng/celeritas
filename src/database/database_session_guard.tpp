#pragma once

#include "database_session_guard.h"
#include "common/common_fwd.h"
#include "common/noexcept_safe_call_and_log.h"

template <typename SessionType>
celeritas::database_session_guard<SessionType>::database_session_guard(const session_shared_ptr& session, const connection_pool_shared_ptr& pool)
    : session_{ session }, pool_{ pool }
{
}

template <typename SessionType>
celeritas::database_session_guard<SessionType>::~database_session_guard() noexcept
{
    if (pool_ != nullptr && session_ != nullptr)
    {
        noexcept_safe_call_and_log([this] {
                                       pool_->release_session(session_);
                                   },
                                   database_channel,
                                   "error release session: ");
    }
}

template <typename SessionType>
celeritas::database_session_guard<SessionType>::database_session_guard(database_session_guard&& rhs) noexcept
    : session_{ std::move(rhs.session_) }, pool_{ std::move(rhs.pool_) }
{
}

template <typename SessionType>
celeritas::database_session_guard<SessionType>& celeritas::database_session_guard<SessionType>::operator=(database_session_guard&& rhs) noexcept
{
    session_ = std::move(rhs.session_);
    pool_ = std::move(rhs.pool_);

    return *this;
}

template <typename SessionType>
celeritas::database_session_guard<SessionType>::session_shared_ptr celeritas::database_session_guard<SessionType>::get_session()
{
    return session_;
}