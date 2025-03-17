#pragma once

enum class ActorLayer : Type::uint16
{
    kDefault = (0x01<<0),
    kGround = (0x01<<1),
    kPlayer = (0x01<<2),
    kMob = (0x01<<3),
    kBullet = (0x01<<4),
    kBlock = (0x01<<5),
};

ENUM_CLASS_FLAGS(ActorLayer)
