#pragma once

#include "common/worker_pool/thread_safe_queue.h"

#include <memory>

namespace celeritas
{
    class application_loader_base : public std::enable_shared_from_this<application_loader_base>
    {
    public:
        using class_type = application_loader_base;
        using task_type = thread_safe_queue::task_type;

        application_loader_base() noexcept = default;

        virtual ~application_loader_base() noexcept = default;

        application_loader_base(const application_loader_base& rhs) noexcept = default;

        application_loader_base& operator=(const application_loader_base& rhs) noexcept = default;

        application_loader_base(application_loader_base&& rhs) noexcept = default;

        application_loader_base& operator=(application_loader_base&& rhs) noexcept = default;

        virtual void submit_task(task_type task) = 0;
    };
}