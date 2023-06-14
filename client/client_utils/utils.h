#pragma once
#include <chrono>
#include <string>

using namespace std::chrono;
using tp = high_resolution_clock::time_point;

void log_results(tp cur_sample_tp,
                 tp prev_sample_tp,
                 tp oldest_samples_tp,
                 uint32_t samples_count = 1);

double get_duration(tp startTime, tp endTime);

tp get_current_tp();

double convert_hz_to_microseconds(const std::string &frequencyStr);
