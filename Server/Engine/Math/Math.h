#pragma once

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
    bool IsEqual(float a, float b);
    
    int32_t RandRange(int32_t min, int32_t max);
    
    bool IsValid(float f);
}
