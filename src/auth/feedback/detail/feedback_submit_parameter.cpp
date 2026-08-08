#include "feedback_submit_parameter.h"
#include "auth/config/app_secret.h"
#include "common/core/hmac_sha_256.h"
#include "message/basic/game_error_type.h"

#include <boost/algorithm/hex.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/lexical_cast.hpp>
#include <openssl/evp.h>
#include <array>

celeritas::feedback_submit_parameter::feedback_submit_parameter(
    const http_handle_parameter_shared_ptr& http_handle_parameter)
    : base_type{ http_handle_parameter },
      response_{},
      type_{},
      content_{},
      device_info_{},
      server_{},
      player_id_{},
      is_anonymous_{}
{
    init1();
}

int32_t celeritas::feedback_submit_parameter::get_type() const
{
    return type_;
}

std::string celeritas::feedback_submit_parameter::get_content() const
{
    return content_;
}

std::string celeritas::feedback_submit_parameter::get_image_data() const
{
    return image_data_;
}

std::string celeritas::feedback_submit_parameter::get_device_info() const
{
    return device_info_;
}

std::string celeritas::feedback_submit_parameter::get_server() const
{
    return server_;
}

std::string celeritas::feedback_submit_parameter::get_player_id() const
{
    return player_id_;
}

bool celeritas::feedback_submit_parameter::is_anonymous() const
{
    return is_anonymous_;
}

bool celeritas::feedback_submit_parameter::is_failure() const
{
    return response_.has_value();
}

celeritas::http_response celeritas::feedback_submit_parameter::get_response() const
{
    return response_.value();
}

void celeritas::feedback_submit_parameter::init1()
{
    const auto optional_content = get_param("content");
    if (!optional_content || optional_content->empty())
    {
        response_ = http_response{ game_error_type::invalid_parameter, "反馈内容不能为空" };
        return;
    }

    content_ = *optional_content;

    if (const auto optional_type = get_param("type"))
    {
        try
        {
            type_ = boost::lexical_cast<int32_t>(*optional_type);
        }
        catch (...)
        {
            type_ = 0;
        }
    }

    if (const auto optional_image_data = get_param("image_data"))
    {
        image_data_ = *optional_image_data;
    }

    if (const auto optional_device_info = get_param("device_info"))
    {
        device_info_ = *optional_device_info;
    }

    if (const auto optional_server = get_param("server"))
    {
        server_ = *optional_server;
    }

    if (const auto optional_player_id = get_param("player_id"))
    {
        player_id_ = *optional_player_id;
    }

    if (const auto optional_anonymous = get_param("is_anonymous"))
    {
        is_anonymous_ = *optional_anonymous == "1" || *optional_anonymous == "true";
    }

    if (const auto http_response = get_http_parameter())
    {
        response_ = *http_response;
        return;
    }

    const auto secret = app_secret::get_instance().get_key(get_app_id());

    std::string image_hash{};
    if (!image_data_.empty())
    {
        std::array<unsigned char, EVP_MAX_MD_SIZE> md5_buf{};
        unsigned int md5_len{};
        EVP_MD_CTX* ctx = EVP_MD_CTX_new();
        if (ctx)
        {
            EVP_DigestInit_ex(ctx, EVP_md5(), nullptr);
            EVP_DigestUpdate(ctx, image_data_.data(), image_data_.size());
            EVP_DigestFinal_ex(ctx, md5_buf.data(), &md5_len);
            EVP_MD_CTX_free(ctx);

            std::string hex_output{};
            boost::algorithm::hex(md5_buf.data(), md5_buf.data() + md5_len, std::back_inserter(hex_output));
            boost::algorithm::to_lower(hex_output);
            image_hash = hex_output;
        }
    }

    // 签名串需与客户端一致：app_id + type + content + image_hash + device_info + server + player_id + is_anonymous + timestamp
    if (const auto hmac_sha256 = hmac_sha256::calculate_with_args(secret,
                                                                  get_app_id(),
                                                                  type_,
                                                                  content_,
                                                                  image_hash,
                                                                  device_info_,
                                                                  server_,
                                                                  player_id_,
                                                                  is_anonymous_,
                                                                  get_timestamp());
        hmac_sha256 != get_sign())
    {
        response_ = http_response{ game_error_type::sign_error };
    }
}
