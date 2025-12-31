#include "common/core/command_line_config.h"
#include "common/framework/celeritas_main.h"
#include "mock/mock_celeritas_main.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(celeritas_main_suite)

    BOOST_AUTO_TEST_CASE(test_run_calls_create_initializer)
    {
        celeritas::mock_celeritas_main main{ "test_server" };
        char* argv[] = { const_cast<char*>("test_program"), const_cast<char*>("--instance_name=test_instance"), nullptr };
        constexpr auto argc = std::size(argv) - 1;

        main.run(argc, argv);

        BOOST_CHECK(main.is_create_initializer_called());
        BOOST_CHECK_EQUAL(main.get_config_instance_name(), "test_instance");
    }

    BOOST_AUTO_TEST_CASE(test_get_server_type)
    {
        const celeritas::mock_celeritas_main main{ "test_server" };
        BOOST_CHECK_EQUAL(main.get_mock_server_type(), "test_server");
    }

BOOST_AUTO_TEST_SUITE_END()
