#include "pch.h"
#include "Sprite.h"

const Math::Vector2 Sprite::kCenter = Math::Vector2(.5f, .5f);
const Math::Vector2 Sprite::kTopLeft = Math::Vector2(0.f, 1.f);
const Math::Vector2 Sprite::kTop = Math::Vector2(0.f, 1.f);
const Math::Vector2 Sprite::kTopRight = Math::Vector2(1.f, 1.f);
const Math::Vector2 Sprite::kLeft = Math::Vector2(0.f, .5f);
const Math::Vector2 Sprite::kRight = Math::Vector2(1.f, .5f);
const Math::Vector2 Sprite::kBottomLeft = Math::Vector2(0.f, 0.f);
const Math::Vector2 Sprite::kBottom = Math::Vector2(.5f, 0.f);
const Math::Vector2 Sprite::kBottomRight = Math::Vector2(0.f, 1.f);

Sprite::Sprite() :
    frames_(),
    ppu_(32.f)
{
}

bool Sprite::Load(const std::wstring& kPath)
{
    if (!Texture::Load(kPath)) return false;
    
    return true;;
}

void Sprite::Split(Type::uint32 cols, Type::uint32 rows, Math::Vector2 pivot)
{
    const float frame_width = static_cast<float>(width_) / cols;
    const float frame_height = static_cast<float>(height_) / rows;

    frames_.clear();

    for (Type::uint32 y = 0; y < rows; ++y)
    {
        for (Type::uint32 x = 0; x < cols; ++x)
        {
            SpriteFrame frame;
            frame.uv_offset.x = x;
            frame.uv_offset.y = y;
            frame.uv_scale.x = frame_width / static_cast<float>(width_);
            frame.uv_scale.y = frame_height / static_cast<float>(height_);
            frame.pivot = pivot;

            frames_.push_back(frame);
        }
    }
}
