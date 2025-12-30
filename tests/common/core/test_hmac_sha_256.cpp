#include "common/core/hmac_sha_256.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(hmac_sha256_suite)

    // 用于诊断的标准测试用例
    BOOST_AUTO_TEST_CASE(test_with_standard_fox_vector)
    {
        const std::string key = "key";
        const std::string data = "The quick brown fox jumps over the lazy dog";
        const std::string expected = "f7bc83f430538424b13298e6aa6fb143ef4d59a14946175997479dbc2d1a3cd8";
        BOOST_CHECK_EQUAL(celeritas::hmac_sha256::calculate(key, data), expected);
    }

    // 使用RFC 2202中的测试向量进行测试
    BOOST_AUTO_TEST_CASE(test_with_rfc_vectors)
    {
        // Test case 1
        const std::string key1 = "Jefe";
        const std::string data1 = "what do ya want for nothing?";
        const std::string expected1 = "5bdcc146bf60754e6a042426089575c75a003f089d2739839dec58b964ec3843";
        BOOST_CHECK_EQUAL(celeritas::hmac_sha256::calculate(key1, data1), expected1);

        // Test case 2

        // 20 bytes of 0x0b
        const std::string key2(20, 0x0b);
        const std::string data2 = "Hi There";
        const std::string expected2 = "b0344c61d8db38535ca8afceaf0bf12b881dc200c9833da726e9376c2e32cff7";
        BOOST_CHECK_EQUAL(celeritas::hmac_sha256::calculate(key2, data2), expected2);
    }

    // 测试 calculate_with_args 方法
    BOOST_AUTO_TEST_CASE(test_calculate_with_args)
    {
        const std::string secret_key{ "my_secret" };
        const std::string part1{ "hello" };
        constexpr auto part2 = 123;
        constexpr auto part3 = 45.67;
        const std::string combined_data{ "hello12345.67" };

        const auto result1 = celeritas::hmac_sha256::calculate_with_args(secret_key, part1, part2, part3);
        const auto result2 = celeritas::hmac_sha256::calculate(secret_key, combined_data);

        BOOST_CHECK_EQUAL(result1, result2);
    }

    // 测试空字符串和边界情况
    BOOST_AUTO_TEST_CASE(test_empty_and_edge_cases)
    {
        const std::string secret_key{ "a_secret_key" };
        const std::string empty_data{};
        const std::string empty_key{};

        // 测试空数据
        BOOST_CHECK_EQUAL(celeritas::hmac_sha256::calculate(secret_key, empty_data), "225e0bcfb3886a84790ad0ac4f8a9dce85088c77d73ad48add790e1621a30fe7");

        // 测试空密钥
        const std::string data = "some_data";
        BOOST_CHECK_EQUAL(celeritas::hmac_sha256::calculate(empty_key, data), "376399a5b721675ae09e0b56611b57058e32f2d8b940378e5ecb8427aebf0193");

        // 测试空密钥和空数据
        BOOST_CHECK_EQUAL(celeritas::hmac_sha256::calculate(empty_key, empty_data), "b613679a0814d9ec772f95d778c35fc5ff1697c493715653c6c712144292c5ad");
    }

BOOST_AUTO_TEST_SUITE_END()
