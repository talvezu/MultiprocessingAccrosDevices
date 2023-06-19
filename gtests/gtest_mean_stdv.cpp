#define CATCH_CONFIG_MAIN
#include "client_utils/utils.h"
#include <algorithm>
#include <chrono>
#include <deque>
#include <gtest/gtest.h>
#include <random>
#include <thread>

auto random_number_from_normal_distribution = [](double mean,
                                                 double standard_deviation) {
    auto randomFunc =
        [distribution_ =
             std::normal_distribution<double>(mean, standard_deviation),
         random_engine_ = std::mt19937{std::random_device{}()}]() mutable {
            return distribution_(random_engine_);
        };
    return randomFunc;
};

static constexpr int matrix_size{10};
static constexpr int vector_size{1000};
static constexpr double mean{1000};
static constexpr double standard_deviation{50};

TEST(StatisticTests, MeanSTVTest)
{
    std::vector<double> values;
    std::vector<std::vector<double>> matrix;
    auto rand_func =
        random_number_from_normal_distribution(mean, standard_deviation);
    values.reserve(vector_size);

    // prepare data.
    for (int m = 0; m < matrix_size; m++)
    {
        for (int i = 0; i < vector_size; i++)
        {
            values.push_back(rand_func());
        }
        matrix.push_back(values);
        values.clear();
    }

    std::vector<double> mean_vector;
    std::vector<double> stv_vector;
    auto stat_func = [&mean_vector,
                      &stv_vector](const std::vector<double> &vec) {
        mean_vector.push_back(
            std::accumulate(vec.begin(),
                            vec.end(),
                            0,
                            [](double a, double b) { return a + b; }) /
            vector_size);
        stv_vector.push_back(
            calculate_standard_deviation(vec, mean_vector.back()));
    };
    std::for_each(matrix.begin(), matrix.end(), stat_func);

    // validate results
    for_each(mean_vector.begin(), mean_vector.end(), [](double val) {
        ASSERT_NEAR(mean, val, 10.0f);
    });

    for_each(stv_vector.begin(), stv_vector.end(), [](double val) {
        ASSERT_NEAR(standard_deviation, val, 2.0f);
    });
}
