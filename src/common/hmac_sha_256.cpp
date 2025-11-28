#include "hmac_sha_256.h"

#include <boost/algorithm/hex.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <openssl/evp.h>
#include <openssl/hmac.h>

#include <array>

std::string celeritas::hmac_sha256::calculate(const std::string& secret_key, const std::string& data)
{
    std::array<unsigned char, EVP_MAX_MD_SIZE> result{};
    unsigned int result_length{};

    // HMAC 计算
    // 参数: 算法, Key, Key长度, 数据, 数据长度, 输出Buffer, 输出长度指针
    HMAC(EVP_sha256(),
         secret_key.c_str(), static_cast<int>(secret_key.size()),
         reinterpret_cast<const unsigned char*>(data.c_str()), data.length(),
         result.data(), &result_length);

    // Hex 转换
    std::string hex_output{};
    boost::algorithm::hex(result.data(), result.data() + result_length, std::back_inserter(hex_output));
    boost::algorithm::to_lower(hex_output);

    return hex_output;
}