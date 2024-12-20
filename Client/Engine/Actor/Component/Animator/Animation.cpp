#include "pch.h"
#include "Animation.h"

Animation::Animation(const std::wstring& kName) :
    name_(kName),
    frame_rate_(0),
    is_loop_(false),
    frames_()
{
}
