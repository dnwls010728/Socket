#pragma once
#include "Misc/EnumClassFlags.h"

enum class ActorLayer : uint16_t
{
    kDefault = (0x01<<0),
    kCharacter = (0x01<<1),
};

ENUM_CLASS_FLAGS(ActorLayer)
