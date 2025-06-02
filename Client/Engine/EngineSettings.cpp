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

void EngineSettings::SetScreenSize(uint32_t width, uint32_t height)
{
    screen_width_ = width;
    screen_height_ = height;
}
