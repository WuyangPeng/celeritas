#pragma once

#include "common/framework/resource_loader_base.h"
#include "proto/service/log.pb.h"

#include <boost/asio/any_io_executor.hpp>
#include <boost/asio/steady_timer.hpp>
#include <memory>
#include <mutex>
#include <vector>
#include <string>

namespace celeritas
{
    class log_client
    {
    public:
        using class_type = log_client;
        using any_io_executor = boost::asio::any_io_executor;
        using resource_loader_shared_ptr = std::shared_ptr<resource_loader_base>;
        using service_log = proto::service::service_log;

        static log_client& get_instance();

        void initialize(const any_io_executor& any_io_executor, const resource_loader_shared_ptr& resource_loader);

        void stop();

        void add_log(const service_log& log);

    private:
        using steady_timer = boost::asio::steady_timer;
        using steady_timer_unique_ptr = std::unique_ptr<steady_timer>;
        using service_log_container = std::vector<service_log>;

        log_client() = default;

        void start_flush_timer();

        void flush_logs();

        any_io_executor executor_;
        resource_loader_shared_ptr resource_loader_;
        steady_timer_unique_ptr timer_;

        std::mutex mutex_;
        service_log_container buffer_;
    };
}
