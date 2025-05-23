#pragma once

#include <Windows.h>

class Time
{
public:
    Time();
    ~Time() = default;

    static double Init();
    static double Seconds();

private:
    static double second_per_count_;
    
};
