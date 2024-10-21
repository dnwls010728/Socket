#pragma once

enum class LevelType : Type::uint64
{
    kMainMenu = 0,
    kDefault,
    kEnd
};

enum class EndPlayReason : Type::uint64
{
    kDestroyed,
    kLevelTransition,
    kQuit
};
