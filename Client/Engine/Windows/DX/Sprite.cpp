#include "pch.h"
#include "Sprite.h"

#include "Data/FileHelper.h"

const Math::Vector2 Sprite::kCenter = Math::Vector2(.5f, .5f);
const Math::Vector2 Sprite::kTopLeft = Math::Vector2(0.f, 1.f);
const Math::Vector2 Sprite::kTop = Math::Vector2(.5f, 1.f);
const Math::Vector2 Sprite::kTopRight = Math::Vector2(1.f, 1.f);
const Math::Vector2 Sprite::kLeft = Math::Vector2(0.f, .5f);
const Math::Vector2 Sprite::kRight = Math::Vector2(1.f, .5f);
const Math::Vector2 Sprite::kBottomLeft = Math::Vector2(0.f, 0.f);
const Math::Vector2 Sprite::kBottom = Math::Vector2(.5f, 0.f);
const Math::Vector2 Sprite::kBottomRight = Math::Vector2(1.f, 0.f);

Sprite::Sprite() :
    frames_(),
    ppu_(32)
{
}

bool Sprite::Load(const std::wstring& kPath)
{
    if (!Texture::Load(kPath)) return false;

    if (!meta_data_.IsNull())
    {
        wrap_mode_ = static_cast<WrapMode>(meta_data_["wrap_mode"].as<Type::uint8>());
        filter_mode_ = static_cast<FilterMode>(meta_data_["filter_mode"].as<Type::uint8>());
        
        ppu_ = meta_data_["ppu"].as<Type::uint32>();

        if (meta_data_["frames"].IsSequence())
        {
            for (const auto& frame : meta_data_["frames"])
            {
                SpriteFrame sprite_frame;
                sprite_frame.uv_offset.x = frame["rect"]["x"].as<float>();
                sprite_frame.uv_offset.y = frame["rect"]["y"].as<float>();
                sprite_frame.uv_scale.x = frame["rect"]["width"].as<float>();
                sprite_frame.uv_scale.y = frame["rect"]["height"].as<float>();
                sprite_frame.pivot.x = frame["pivot"]["x"].as<float>();
                sprite_frame.pivot.y = frame["pivot"]["y"].as<float>();
                
                std::string name = frame["name"].as<std::string>();
                std::wstring to_wstring = std::wstring(name.begin(), name.end());
                frames_[to_wstring] = sprite_frame;
            }
        }
    }
    else
    {
        frames_.clear();

        SpriteFrame frame;
        frame.uv_offset = Math::Vector2::Zero();
        frame.uv_scale = Math::Vector2::One();
        frame.pivot = kCenter;

        std::wstring filename = FileHelper::GetFilenameWithoutExtension(kPath);
        frames_[filename + L"_0"] = frame;
    }
    
    return true;;
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Sprite>("Sprite")
        .constructor<>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
