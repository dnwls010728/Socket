#include "pch.h"
#include "Math.h"

#include <numbers>
#include <random>

float Math::Sign(float a)
{
    if (a < 0.f) return -1.f;
    return 1.f;
}

float Math::Lerp(float a, float b, float t)
{
    return a + (b - a) * t;
}

float Math::Min(float a, float b)
{
    return a < b ? a : b;
}

float Math::Max(float a, float b)
{
    return a > b ? a : b;
}

float Math::Clamp(float value, float min, float max)
{
    return Max(min, Min(value, max));
}

float Math::Clamp01(float value)
{
    return Clamp(value, 0.f, 1.f);
}

float Math::Abs(float a)
{
    return a < 0.f ? -a : a;
}

float Math::Pow(float f, float p)
{
    float result = 1.f;

    for (int32_t i = 0; i < p; ++i)
    {
        result *= f;
    }
    
    return result;
}

float Math::Rad2Deg()
{
    return 360.f / (Math::PI() * 2.f);
}

float Math::Deg2Rad()
{
    return Math::PI() / 180.f;
}

float Math::RandRange(float min, float max)
{
    if (min > max)
    {
        float temp = min;
        min = max;
        max = temp;
    }
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);

    return dis(gen);
}

float Math::PI()
{
    return std::numbers::pi_v<float>;
}

int32_t Math::RandRange(int32_t min, int32_t max)
{
    if (min > max)
    {
        float temp = min;
        min = max;
        max = temp;
    }
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int32_t> dis(min, max);

    return dis(gen);
}

bool Math::IsEqual(float a, float b)
{
    return std::abs(a - b) < std::numeric_limits<float>::epsilon();
}

bool Math::IsValid(float f)
{
    return !isnan(f) && !isinf(f);
}
