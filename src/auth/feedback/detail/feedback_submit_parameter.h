#pragma once

#include "auth/core/detail/auth_parameter.h"
#include "message/basic/http_response.h"

namespace celeritas
{
    class feedback_submit_parameter final : public auth_parameter
    {
    public:
        using class_type = feedback_submit_parameter;
        using base_type = auth_parameter;

        explicit feedback_submit_parameter(const http_handle_parameter_shared_ptr& http_handle_parameter);

        [[nodiscard]] int32_t get_type() const;

        [[nodiscard]] std::string get_content() const;

        [[nodiscard]] std::string get_image_data() const;

        [[nodiscard]] std::string get_device_info() const;

        [[nodiscard]] std::string get_server() const;

        [[nodiscard]] std::string get_player_id() const;

        [[nodiscard]] bool is_anonymous() const;

        [[nodiscard]] bool is_failure() const override;

        [[nodiscard]] http_response get_response() const;

    private:
        void init1();

        optional_http_response response_;
        int32_t type_;
        std::string content_;
        std::string image_data_;
        std::string device_info_;
        std::string server_;
        std::string player_id_;
        bool is_anonymous_;
    };
}
