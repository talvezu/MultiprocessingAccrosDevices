#define CATCH_CONFIG_MAIN
#include "client_utils/utils.h"
#include <chrono>
#include <deque>
#include <gtest/gtest.h>
#include <thread>

std::deque<tp> time_of_arrivals;

using namespace std::chrono;
using tp = high_resolution_clock::time_point;

/**
 * using namespace std::chrono;


void log_results(tp cur_sample_tp,
                 tp prev_sample_tp,
                 tp oldest_samples_tp,
                 uint32_t samples_count = 1);

double get_duration(tp startTime, tp endTime);

tp get_current_tp();

double convert_hz_to_microseconds(const std::string &frequencyStr);
*/
TEST(TimingTests, HZTest)
{
    double micros = convert_hz_to_microseconds(std::to_string(1000));
    ASSERT_NEAR(micros, 1000.0f, 0.01f);

    micros = convert_hz_to_microseconds(std::to_string(1000000));
    ASSERT_NEAR(micros, 1.0f, 0.01f);

    micros = convert_hz_to_microseconds(std::to_string(1));
    ASSERT_NEAR(micros, 1000000.0f, 0.01f);
}


TEST(TimingTests, DeltaTest)
{
    tp before_sleep = get_current_tp();
    std::this_thread::sleep_for(100ms);
    tp after_sleep = get_current_tp();
    // Calculate the duration between the time points
    std::chrono::duration<double, std::micro> duration =
        after_sleep - before_sleep;

    // Access the duration in different units
    double milliseconds = duration.count() / 1000; // Duration in milliseconds
    double microseconds = duration.count();        // Duration in microseconds
    ASSERT_NEAR(milliseconds, 100.0f, 0.2f);
}

TEST(TimingTests, DeltaDeque)
{

    uint32_t samples_window_span = 100;
    tp now_tp = get_current_tp();
    double delta_in_microseconds =
        convert_hz_to_microseconds(std::to_string(1000));
    microseconds duration_to_add(
        static_cast<long long>(10 /*delta_in_microseconds*/));
    std::chrono::microseconds total_duration =
        duration_to_add * samples_window_span;

    for (int i = 0; i < samples_window_span; ++i)
    {
        time_of_arrivals.push_front(now_tp += duration_to_add);
    }
    // we should have 100 sample in deque each 2 delta is 10 micro, so we shold have 10 * 100 = 1000 micros

    tp first = time_of_arrivals.back();
    tp last = time_of_arrivals.front();

    // Calculate the duration between the time points
    std::chrono::duration<double, std::micro> duration = last - first;

    ASSERT_FLOAT_EQ(duration.count(), 990.0f);
}
