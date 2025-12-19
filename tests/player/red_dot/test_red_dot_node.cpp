#include "config/game_config/red_dot_type.h"
#include "player/red_dot/red_dot_node.h"

#include <boost/test/unit_test.hpp>

namespace
{
    struct red_dot_node_fixture
    {
        red_dot_node_fixture()
            : node_{ std::make_shared<celeritas::red_dot_node>(celeritas::red_dot_type::null, false) }
        {
        }

        std::shared_ptr<celeritas::red_dot_node> node_;
    };
}

BOOST_FIXTURE_TEST_SUITE(red_dot_node_suite, red_dot_node_fixture)

    BOOST_AUTO_TEST_CASE(test_initialization)
    {
        BOOST_CHECK_EQUAL(node_->get_value(), 0);
        BOOST_CHECK(node_->is_child());
        BOOST_CHECK(!node_->get_parent().has_value());
        BOOST_CHECK(!node_->is_save_database());
        BOOST_CHECK_EQUAL(static_cast<int>(node_->get_red_dot_type()), static_cast<int>(celeritas::red_dot_type::null));
    }

    BOOST_AUTO_TEST_CASE(test_value_manipulation)
    {
        node_->set_value(10);
        BOOST_CHECK_EQUAL(node_->get_value(), 10);
        node_->add_value(5);
        BOOST_CHECK_EQUAL(node_->get_value(), 15);
        node_->reduce_value(3);
        BOOST_CHECK_EQUAL(node_->get_value(), 12);
    }

    BOOST_AUTO_TEST_CASE(test_parent_child_relationship)
    {
        const auto parent = std::make_shared<celeritas::red_dot_node>(celeritas::red_dot_type::null, false);
        const auto child = std::make_shared<celeritas::red_dot_node>(celeritas::red_dot_type::null, false);

        child->set_parent_node(parent);
        parent->add_child(child);

        BOOST_CHECK(child->is_child());
        BOOST_CHECK(child->get_parent().has_value());
        BOOST_CHECK_EQUAL(child->get_parent().value().get(), parent.get());
    }

BOOST_AUTO_TEST_SUITE_END()
