#pragma once
#include "Misc/EnumClassFlags.h"

enum class ActorLayer : uint16_t
{
    kDefault = (0x01<<0),
    kPortal = (0x01<<1),
    kPlayer = (0x01<<2),
    kNPC = (0x01<<3),
    kMob = (0x01<<4),
    kDroppedItem = (0x01<<5)
};

ENUM_CLASS_FLAGS(ActorLayer)