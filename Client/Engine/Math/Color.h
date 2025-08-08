#pragma once

namespace Math
{
    struct Color
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;

        static Color HexToColor(const std::wstring& hex);
        static std::wstring ColorToHex(Color color);
        static Color HSVToColor(float h, float s, float v);
        static void ColorToHSV(Color color, float& h, float& s, float& v);

        static const Color Red;
        static const Color Green;
        static const Color Blue;
        static const Color White;
        static const Color Black;
        static const Color Gray;
        static const Color Yellow;
        static const Color Cyan;
        static const Color Magenta;
        static const Color Pink;
        static const Color Orange;
    };
}
