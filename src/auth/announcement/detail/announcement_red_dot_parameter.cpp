#include "announcement_red_dot_parameter.h"
#include "auth/config/app_secret.h"
#include "common/core/hmac_sha_256.h"
#include "message/basic/game_error_type.h"

#include <boost/lexical_cast.hpp>

celeritas::announcement_red_dot_parameter::announcement_red_dot_parameter(
    const http_handle_parameter_shared_ptr& http_handle_parameter)
    : base_type{ http_handle_parameter }, response_{}, last_time_{}
{
    init1();
}

int64_t celeritas::announcement_red_dot_parameter::get_last_time() const
{
    return last_time_;
}

bool celeritas::announcement_red_dot_parameter::is_failure() const
{
    return response_.has_value();
}

celeritas::http_response celeritas::announcement_red_dot_parameter::get_response() const
{
    return response_.value();
}

void celeritas::announcement_red_dot_parameter::init1()
{
    // last_time 为玩家上次阅读公告时的 UTC Unix 时间戳，缺省按 0 处理
    if (const auto optional_last_time = get_param("last_time"))
    {
        try
        {
            last_time_ = boost::lexical_cast<int64_t>(*optional_last_time);
        }
        catch (...)
        {
            last_time_ = 0;
        }
    }

    if (const auto http_response = get_http_parameter())
    {
        response_ = *http_response;
        return;
    }

    const auto secret = app_secret::get_instance().get_key(get_app_id());

    // 签名串需与客户端一致：app_id + last_time + timestamp（均按字符串拼接）
    if (const auto hmac_sha256 = hmac_sha256::calculate_with_args(secret, get_app_id(), last_time_, get_timestamp());
        hmac_sha256 != get_sign())
    {
        response_ = http_response{ game_error_type::sign_error };
    }
}
