#pragma once

#define MATH_PI 3.14159265358979323846f

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
    
    int RandRange(int min, int max);
    
    bool IsValid(float f);
}
