#pragma once

#include "item_flow_log_param.h"
#include "log_param.h"
#include "proto/service/log.pb.h"

namespace celeritas
{
    class item_log_helper
    {
    public:
        using class_type = item_log_helper;

        static void add_item_flow_log(const log_param& param, const item_flow_log_param& item_param);

    private:
        using service_log_type = proto::service::service_log;

        static void fill_log_header(service_log_type& log, const log_param& param);

        static void fill_item_flow_log(service_log_type& log, const item_flow_log_param& item_param);
    };
}
