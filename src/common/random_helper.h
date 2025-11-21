#pragma once

#include "config/app_config.h"
#include "handler/auth/guest_login_http_message_handler.h"

#include <random>

namespace celeritas
{
    class random_helper
    {
    public:
        using class_type = random_helper;

        // 生成 [0, end) 范围内的 int
        [[nodiscard]] static int get_random_int(int end);

        // 生成 [begin, end) 范围内的 int
        [[nodiscard]] static int get_random_int(int begin, int end);

        // 生成 [0.0f, 1.0f) 范围内的 float
        [[nodiscard]] static float get_random_float();

        // 生成 [begin, end) 范围内的 float
        [[nodiscard]] static float get_random_float(float begin, float end);

        // 生成 [0.0, 1.0) 范围内的 double
        [[nodiscard]] static double get_random_double();

        // 生成 [begin, end) 范围内的 double
        [[nodiscard]] static double get_random_double(double begin, double end);

        // 生成布尔值，p 是返回 true 的概率 (默认为 0.5)
        [[nodiscard]] static bool get_random_bool(double p = 0.5);

        // 生成服从正态分布的 double
        // mean: 均值 (默认为 0.0)
        // stddev: 标准差 (默认为 1.0)
        [[nodiscard]] static double get_random_normal(double mean = 0.0, double stddev = 1.0);

    private:
        [[nodiscard]] static std::mt19937& get_engine();
    };
}