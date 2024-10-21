#pragma once
#include "Misc/Type.h"

namespace Math
{
    struct Color
    {
        Type::uint8 r;
        Type::uint8 g;
        Type::uint8 b;
        Type::uint8 a;

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
    };
}
