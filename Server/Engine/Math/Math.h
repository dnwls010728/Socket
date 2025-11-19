#pragma once
#include <cstdint>

namespace Math
{
    float Sign(float a);
    float Lerp(float a, float b, float t);
    float Min(float a, float b);
    float Max(float a, float b);
    float Clamp(float value, float min, float max);
    float Clamp01(float value);
    float Abs(float a);
    float Pow(float f, float p);
    float Rad2Deg();
    float Deg2Rad();
    float RandRange(float min, float max);
    float PI();

    int32_t Sign(int32_t a);
    int32_t Min(int32_t a, int32_t b);
    int32_t Max(int32_t a, int32_t b);
    int32_t Clamp(int32_t value, int32_t min, int32_t max);
    int32_t Clamp01(int32_t value);
    int32_t Abs(int32_t a);
    int32_t Pow(int32_t f, int32_t p);
    int32_t RandRange(int32_t min, int32_t max);
    
    bool IsEqual(float a, float b);
    bool IsValid(float f);
}
