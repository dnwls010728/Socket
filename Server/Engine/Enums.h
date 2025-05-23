#pragma once
#include "Engine/Misc/Type.h"
enum class EndPlayReason : Type::uint64
{
    kDestroyed,
    kLevelTransition,
    kQuit
};
