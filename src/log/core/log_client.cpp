#include "log_client.h"
#include "log/log_constant.h"
#include "initializer/initializer_constant.h"
#include "message/basic/header.h"
#include "common/common_constant.h"
#include "common/logging/logger.h"
#include "proto/celeritas.pb.h"
#include "proto/common/common.pb.h"

celeritas::log_client& celeritas::log_client::get_instance()
{
    static log_client instance;
    return instance;
}

void celeritas::log_client::initialize(const any_io_executor& any_io_executor, const resource_loader_shared_ptr& resource_loader)
{
    executor_ = any_io_executor;
    resource_loader_ = resource_loader;
    timer_ = std::make_unique<boost::asio::steady_timer>(executor_);
    start_flush_timer();

    LOG_CHANNEL(log_channel, info) << "log_client initialized with flush interval of " << log_flush_interval.count() << " seconds.";
}

void celeritas::log_client::stop()
{
    if (timer_)
    {
        timer_->cancel();
    }
    flush_logs();
}

void celeritas::log_client::add_log(const service_log& log)
{
    std::lock_guard<std::mutex> lock(mutex_);
    buffer_.push_back(log);
}

void celeritas::log_client::start_flush_timer()
{
    if (!timer_)
        return;

    timer_->expires_after(log_flush_interval);
    timer_->async_wait([this](const boost::system::error_code& error) {
        if (error)
        {
            return;
        }
        flush_logs();
        start_flush_timer();
    });
}

void celeritas::log_client::flush_logs()
{
    std::vector<service_log> temp;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (buffer_.empty())
        {
            return;
        }
        std::swap(temp, buffer_);
    }

    if (!resource_loader_)
    {
        LOG_CHANNEL(log_channel, warning) << "log_client resource_loader is null, logs dropped.";
        return;
    }

    proto::celeritas request{};
    auto* service_request = request.mutable_celeritas_request()->mutable_service();
    auto* log_request = service_request->mutable_log();
    for (const auto& log : temp)
    {
        *log_request->add_service_log() = log;
    }

    header header{ proto::common::empty_message_header{} };
    if (resource_loader_->write_to_server(log_type.data(), header, request))
    {
        LOG_CHANNEL(log_channel, trace) << "flushed " << temp.size() << " logs to log server.";
    }
    else
    {
        LOG_CHANNEL(log_channel, warning) << "failed to flush logs to log server.";
    }
}
