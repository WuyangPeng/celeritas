#include "config/game/container_config.h"
// 必须包含 tpp 文件因为是模板实现
#include "config/game/container_config.tpp"
#include "common/core/celeritas_error.h"

#include <boost/test/unit_test.hpp>
#include <string>
#include <memory>

// 用于测试的辅助类，模拟配置对象
struct MockConfigElement {
    int id;
    std::string value;

    MockConfigElement(int i, std::string v) : id(i), value(std::move(v)) {}

    // container_config 要求元素必须有 get_id() 方法
    [[nodiscard]] int get_id() const { return id; }
};

BOOST_AUTO_TEST_SUITE(container_config_suite)

    BOOST_AUTO_TEST_CASE(test_add_and_get)
    {
        celeritas::container_config<MockConfigElement> container;

        auto elem1 = std::make_shared<MockConfigElement>(1, "one");
        auto elem2 = std::make_shared<MockConfigElement>(2, "two");

        container.add_config(elem1);
        container.add_config(elem2);

        auto result1 = container.get(1);
        BOOST_CHECK(result1.has_value());
        BOOST_CHECK_EQUAL((*result1)->value, "one");

        auto result2 = container.get(2);
        BOOST_CHECK(result2.has_value());
        BOOST_CHECK_EQUAL((*result2)->value, "two");

        auto result3 = container.get(3);
        BOOST_CHECK(!result3.has_value());
    }

    BOOST_AUTO_TEST_CASE(test_duplicate_id_throws)
    {
        celeritas::container_config<MockConfigElement> container;

        auto elem1 = std::make_shared<MockConfigElement>(1, "first");
        auto elem2 = std::make_shared<MockConfigElement>(1, "duplicate");

        container.add_config(elem1);

        BOOST_CHECK_THROW(container.add_config(elem2), celeritas::celeritas_error);
    }

    BOOST_AUTO_TEST_CASE(test_get_container)
    {
        celeritas::container_config<MockConfigElement> container;

        auto elem1 = std::make_shared<MockConfigElement>(10, "ten");
        container.add_config(elem1);

        const auto& map = container.get_container();
        BOOST_CHECK_EQUAL(map.size(), 1);
        BOOST_CHECK_EQUAL(map.at(10)->value, "ten");
    }

BOOST_AUTO_TEST_SUITE_END()
