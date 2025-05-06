#pragma once

enum class EndPlayReason : Type::uint64
{
    kDestroyed,
    kLevelTransition,
    kQuit
};
