#include "pch.h"
#include "Sprite.h"

#include "Data/FileHelper.h"
#include "Misc/StringHelper.h"

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
    frame_indexes_(),
    frames_(),
    ppu_(32)
{
}

bool Sprite::Load(const std::wstring& kPath)
{
    if (!Texture::Load(kPath)) return false;

    if (!meta_data_.IsNull())
    {
        wrap_mode_ = static_cast<WrapMode>(meta_data_["wrap_mode"].as<uint8_t>());
        filter_mode_ = static_cast<FilterMode>(meta_data_["filter_mode"].as<uint8_t>());
        
        ppu_ = meta_data_["ppu"].as<uint32_t>();

        if (meta_data_["frames"].IsSequence())
        {
            for (const auto& frame : meta_data_["frames"])
            {
                SpriteFrame sprite_frame = frame.as<SpriteFrame>();
                frames_.push_back(sprite_frame);
                
                std::wstring name = StringHelper::UTF8ToUTF16(frame["name"].as<std::string>());
                frame_indexes_[name] = frames_.size() - 1;
            }
        }
    }
    else
    {
        frame_indexes_.clear();

        SpriteFrame frame;
        frame.uv_offset = Math::Vector2::Zero();
        frame.uv_scale = Math::Vector2::One();
        frame.pivot = kCenter;

        frames_.push_back(frame);

        std::wstring filename = FileHelper::GetFilenameWithoutExtension(kPath);
        frame_indexes_[filename + L"_0"] = frames_.size() - 1;
    }
    
    return true;
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Sprite>("Sprite")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
