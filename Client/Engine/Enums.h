#pragma once

enum class LevelType : MathTypes::uint64
{
    kMainMenu = 0,
    kDefault,
    kEnd
};

enum class EndPlayReason : MathTypes::uint64
{
    kDestroyed,
    kLevelTransition,
    kQuit
};
