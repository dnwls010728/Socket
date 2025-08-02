#include "pch.h"
#include "Color.h"

#include <format>
#include <iomanip>
#include <sstream>

#include "Math.h"

const Math::Color Math::Color::Red = Math::Color(255, 0, 0, 255);
const Math::Color Math::Color::Green = Math::Color(0, 255, 0, 255);
const Math::Color Math::Color::Blue = Math::Color(0, 0, 255, 255);
const Math::Color Math::Color::White = Math::Color(255, 255, 255, 255);
const Math::Color Math::Color::Black = Math::Color(0, 0, 0, 255);
const Math::Color Math::Color::Gray = Math::Color(128, 128, 128, 255);
const Math::Color Math::Color::Yellow = Math::Color(255, 255, 0, 255);
const Math::Color Math::Color::Cyan = Math::Color(0, 255, 255, 255);
const Math::Color Math::Color::Magenta = Math::Color(255, 0, 255, 255);
const Math::Color Math::Color::Pink = Math::Color(255, 192, 203, 255);
const Math::Color Math::Color::Orange = Math::Color(255, 165, 0, 255);

Math::Color Math::Color::HexToColor(const std::wstring& hex)
{
    uint32_t value = 0;
    std::wstringstream ss;
    ss << std::hex << hex;
    ss >> value;

    Color color;
    color.r = static_cast<uint8_t>((value >> 16) & 0xFF);
    color.g = static_cast<uint8_t>((value >> 8) & 0xFF);
    color.b = static_cast<uint8_t>(value & 0xFF);
    color.a = 255;
    
    return color;
}

std::wstring Math::Color::ColorToHex(Color color)
{
    std::wstring hex = std::format(L"{:02X}{:02X}{:02X}",
                                   static_cast<uint32_t>(color.r),
                                   static_cast<uint32_t>(color.g),
                                   static_cast<uint32_t>(color.b));
    return hex;
}

Math::Color Math::Color::HSVToColor(float h, float s, float v)
{
    Color result;

    if (IsEqual(s, 0.f))
    {
        result.r = static_cast<uint8_t>(v * 255);
        result.g = static_cast<uint8_t>(v * 255);
        result.b = static_cast<uint8_t>(v * 255);
        result.a = 255;
        return result;
    }

    h = IsEqual(h, 360.f) ? 0.f : h / 60.f;
    int32_t i = static_cast<int32_t>(std::trunc(h));
    
    float f = h - i;
    float p = v * (1.f - s);
    float q = v * (1.f - f * s);
    float t = v * (1.f - (1.f - f) * s);

    switch (i)
    {
        case 0:
            result.r = static_cast<uint8_t>(v * 255);
            result.g = static_cast<uint8_t>(t * 255);
            result.b = static_cast<uint8_t>(p * 255);
            result.a = 255;
            break;
        case 1:
            result.r = static_cast<uint8_t>(q * 255);
            result.g = static_cast<uint8_t>(v * 255);
            result.b = static_cast<uint8_t>(p * 255);
            result.a = 255;
            break;
        case 2:
            result.r = static_cast<uint8_t>(p * 255);
            result.g = static_cast<uint8_t>(v * 255);
            result.b = static_cast<uint8_t>(t * 255);
            result.a = 255;
            break;
        case 3:
            result.r = static_cast<uint8_t>(p * 255);
            result.g = static_cast<uint8_t>(q * 255);
            result.b = static_cast<uint8_t>(v * 255);
            result.a = 255;
            break;
        case 4:
            result.r = static_cast<uint8_t>(t * 255);
            result.g = static_cast<uint8_t>(p * 255);
            result.b = static_cast<uint8_t>(v * 255);
            result.a = 255;
            break;
        default:
            result.r = static_cast<uint8_t>(v * 255);
            result.g = static_cast<uint8_t>(p * 255);
            result.b = static_cast<uint8_t>(q * 255);
            result.a = 255;
            break;
    }

    return result;
}
