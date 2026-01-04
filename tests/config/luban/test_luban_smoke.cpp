#include "config/luban/generated/schema.h"

#include <boost/test/unit_test.hpp>

#include <filesystem>
#include <string>

namespace
{
    [[nodiscard]] std::filesystem::path get_config_bin_path()
    {
        const std::filesystem::path source_file_path{ __FILE__ };
        const auto project_root = source_file_path.parent_path().parent_path().parent_path().parent_path();
        return project_root / "config" / "bin";
    }

    [[nodiscard]] auto create_file_loader(const std::filesystem::path& base_path)
    {
        return [base_path](luban::ByteBuf& buf, const std::string& file_name) -> bool {
            buf.clear();
            const auto file_path = base_path / (file_name + ".bytes");

            if (!std::filesystem::exists(file_path))
            {
                BOOST_FAIL("Luban config file not found: " << file_path.string());
                return false;
            }

            return buf.loadFromFile(file_path.string());
        };
    }
}

BOOST_AUTO_TEST_SUITE(luban_smoke_suite)

    BOOST_AUTO_TEST_CASE(test_load_all_tables_from_real_files)
    {
        const auto base_path = get_config_bin_path();
        auto loader = create_file_loader(base_path);

        const auto tables = std::make_shared<celeritas::config::tables>();

        BOOST_CHECK_NO_THROW(tables->load(loader));
        BOOST_CHECK(tables->load(loader) == true);
    }

BOOST_AUTO_TEST_SUITE_END()
