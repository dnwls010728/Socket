#pragma once

enum class EndPlayReason : uint64_t
{
    kDestroyed,
    kLevelTransition,
    kQuit
};
