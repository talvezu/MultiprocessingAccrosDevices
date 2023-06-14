#include "utils.h"
#include <fmt/format.h>
#include <iostream>
#include <spdlog/spdlog.h>

using std::milli;
/*
void log_results(tp cur_sample_tp,
                 tp prev_sample_tp,
                 tp oldest_samples_tp,
                 uint32_t samples_count)
{
    auto duration_from_last_sample =
        get_duration(cur_sample_tp, prev_sample_tp);
    auto duration_from_oldest_sample =
        get_duration(cur_sample_tp, oldest_samples_tp);
    if (samples_count > 1)
    {
        duration_from_oldest_sample =
            get_duration(cur_sample_tp, oldest_samples_tp);
    }

    auto span_message = fmt::format("");
    if (samples_count > 1)
    {
        auto samples_per_second =
            (duration_from_oldest_sample * 1000000) / samples_count;
        span_message += fmt::format(
            "span over {} samples, rate is of {} samples per second",
            samples_count,
            samples_per_second);
    }
    auto log_message =
        fmt::format("{}\n time from prev samples is {} microseconds",
                    span_message,
                    duration_from_last_sample);

    spdlog::info(log_message);
}
*/
void log_results(tp cur_sample_tp,
                 tp prev_sample_tp,
                 tp oldest_samples_tp,
                 uint32_t samples_count)
{
    auto duration_from_last_sample =
        get_duration(prev_sample_tp, cur_sample_tp);
    auto duration_from_oldest_sample =
        get_duration(oldest_samples_tp, cur_sample_tp);

    auto span_message = std::string{};
    if (samples_count > 1)
    {
        auto samples_per_second = static_cast<double>(samples_count) /
                                  (duration_from_oldest_sample * 1e-6);
        span_message += fmt::format(
            "span over {} samples, rate is of {} samples per second",
            samples_count,
            samples_per_second);
    }
    auto log_message =
        fmt::format("{}\n time from prev samples is {} microseconds",
                    span_message,
                    duration_from_last_sample);

    spdlog::info(log_message);
}
double get_duration(tp start_time, tp end_time)
{
    return std::chrono::duration_cast<std::chrono::microseconds>(end_time -
                                                                 start_time)
        .count();
}

tp get_current_tp()
{
    return high_resolution_clock::now();
}

double convert_hz_to_microseconds(const std::string &frequencyStr)
{
    double frequency = std::stod(frequencyStr);
    double microseconds = 1 / (frequency * 1e-6);
    return microseconds;
}
