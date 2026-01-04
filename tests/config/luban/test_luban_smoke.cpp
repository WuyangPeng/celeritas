#include "config/luban/generated/schema.h"

#include <boost/test/unit_test.hpp>

#include <filesystem>
#include <string>

// 用于验证 Luban 生成的真实数据可以被成功加载
BOOST_AUTO_TEST_SUITE(luban_smoke_suite)

    // 加载所有配置表
    // 确保程序能正确解析策划提供的所有最新 .bytes 文件，而不会因格式或数据问题崩溃。
    BOOST_AUTO_TEST_CASE(test_load_all_tables_from_real_files)
    {
        // 使用 __FILE__ 宏和 std::filesystem 来定位项目中的数据目录
        const std::filesystem::path source_file_path{ __FILE__ };
        const auto project_root = source_file_path.parent_path().parent_path().parent_path().parent_path();
        const auto base_path = project_root / "config" / "bin";

        // 创建一个加载器 lambda 函数，它会根据表名拼接完整的文件路径并加载文件内容
        auto loader = [&](luban::ByteBuf& buf, const std::string& file_name) -> bool {
            // 清空 ByteBuf 以接收新数据
            buf.clear();

            // 构建配置文件的完整路径
            const auto file_path = base_path / (file_name + ".bytes");

            // 检查文件是否存在
            if (!std::filesystem::exists(file_path))
            {
                // 在测试输出中明确指出哪个文件缺失了
                BOOST_FAIL("Luban config file not found: " << file_path.string());
                return false;
            }

            // 从文件加载二进制数据到 ByteBuf
            return buf.loadFromFile(file_path.string());
        };

        // 创建一个空的 tables 对象
        const auto tables = std::make_shared<celeritas::config::tables>();

        BOOST_CHECK_NO_THROW(tables->load(loader));
        BOOST_CHECK(tables->load(loader) == true);
    }

BOOST_AUTO_TEST_SUITE_END()
