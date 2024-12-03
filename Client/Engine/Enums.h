#pragma once

enum class LevelType : Type::uint64
{
    kEditor = 0,
    kMainMenu,
    kDefault,
    kEnd
};

enum class EndPlayReason : Type::uint64
{
    kDestroyed,
    kLevelTransition,
    kQuit
};
