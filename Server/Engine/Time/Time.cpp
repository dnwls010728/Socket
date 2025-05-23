#include "pch.h"
#include "Time.h"

double Time::second_per_count_ = 0.f;

Time::Time()
{
}

double Time::Init()
{
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    second_per_count_ = 1.f / static_cast<double>(frequency.QuadPart);

    return Seconds();
}

double Time::Seconds()
{
    LARGE_INTEGER count;
    QueryPerformanceCounter(&count);

    return static_cast<double>(count.QuadPart) * second_per_count_;
}
