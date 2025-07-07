#include "pch.h"
#include "TimedBool.h"

#include "Math/Math.h"

TimedBool::TimedBool() :
    duration_(0.f),
    remaining_(0.f),
    value_(false)
{
}

TimedBool& TimedBool::operator=(bool value)
{
    value_ = value;
    remaining_ = 0.f;
    duration_ = 0.f;
    
    return *this;
}

void TimedBool::Set(float seconds)
{
    duration_ = seconds;
    remaining_ = seconds;
    value_ = true;
}

void TimedBool::Tick(float delta_time)
{
    if (!value_) return;
    if (remaining_ <= 0.f)
    {
        value_ = false;
        remaining_ = 0.f;
        return;
    }
    
    remaining_ -= delta_time;
}

float TimedBool::GetAlpha() const
{
    return 1.f - Math::Clamp01(remaining_ / duration_);
}
