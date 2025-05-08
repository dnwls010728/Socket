#pragma once
#include "Misc/EnumClassFlags.h"
#include "Misc/Type.h"

enum class ActorLayer : Type::uint16
{
    kDefault = (0x01<<0),
    kCharacter = (0x01<<1),
};

ENUM_CLASS_FLAGS(ActorLayer)
