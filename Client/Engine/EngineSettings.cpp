#include "pch.h"
#include "EngineSettings.h"

EngineSettings::EngineSettings() :
    window_title_(L"Untitled"),
    default_level_(L""),
    screen_width_(640),
    screen_height_(480),
    use_vsync_(false),
    fixed_time_step_(.02f),
    layer_collision_matrix_()
{
}

void EngineSettings::SetScreenSize(Type::uint32 width, Type::uint32 height)
{
    screen_width_ = width;
    screen_height_ = height;
}
