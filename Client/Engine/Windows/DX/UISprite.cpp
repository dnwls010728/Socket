#include "pch.h"
#include "UISprite.h"

#include "Data/FileHelper.h"
#include "Level/World.h"
#include "Misc/StringHelper.h"

UISprite::UISprite() :
    bitmap_(nullptr),
    width_(0),
    height_(0),
    filter_mode_(FilterMode::kBilinear),
    frames_(),
    slice9_rect_(Math::Rect::Zero())
{
}

bool UISprite::Load(const std::wstring& path)
{
    Asset::Load(path);

    if (!LoadBitmap(path)) return false;
    
    if (!meta_data_.IsNull())
    {
        filter_mode_ = static_cast<FilterMode>(meta_data_["filter_mode"].as<uint8_t>());

        if (meta_data_["frames"].IsSequence())
        {
            for (const auto& frame : meta_data_["frames"])
            {
                UISpriteFrame sprite_frame = frame.as<UISpriteFrame>();
                frames_.push_back(sprite_frame);
                
                std::wstring name = StringHelper::UTF8ToUTF16(frame["name"].as<std::string>());
                frame_indexes_[name] = frames_.size() - 1;
            }
        }
    }
    else
    {
        UISpriteFrame frame;
        frame.offset = Math::Vector2::Zero();
        frame.size = { static_cast<float>(width_), static_cast<float>(height_) };
        frame.border_min = Math::Vector2::Zero();
        frame.border_max = Math::Vector2::Zero();
        frames_.push_back(frame);

        std::wstring filename = FileHelper::GetFilenameWithoutExtension(path);
        frame_indexes_[filename + L"_0"] = frames_.size() - 1;
    }
    
    return true;
}

uint32_t UISprite::GetWidth(const std::wstring& frame_name) const
{
    auto it = frame_indexes_.find(frame_name);
    if (it != frame_indexes_.end())
    {
        const UISpriteFrame& frame = frames_[it->second];
        return static_cast<uint32_t>(frame.size.x);
    }

    return 0;
}

uint32_t UISprite::GetHeight(const std::wstring& frame_name) const
{
    auto it = frame_indexes_.find(frame_name);
    if (it != frame_indexes_.end())
    {
        const UISpriteFrame& frame = frames_[it->second];
        return static_cast<uint32_t>(frame.size.x);
    }

    return 0;
}

bool UISprite::LoadBitmap(const std::wstring& path)
{
    D2DViewport* d2d_viewport = Renderer::Get()->FindD2DViewport(World::Get()->GetWindow());
    if (!d2d_viewport) return false;
    
    Microsoft::WRL::ComPtr<IWICImagingFactory> factory_ = Renderer::Get()->GetImageFactory();

    Microsoft::WRL::ComPtr<IWICBitmapDecoder> decoder;
    HRESULT hr = factory_->CreateDecoderFromFilename(path.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, decoder.GetAddressOf());
    if (FAILED(hr)) return false;

    Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;
    hr = decoder->GetFrame(0, frame.GetAddressOf());
    if (FAILED(hr)) return false;

    hr = frame->GetSize(&width_, &height_);
    if (FAILED(hr)) return false;

    Microsoft::WRL::ComPtr<IWICFormatConverter> format_converter;
    hr = factory_->CreateFormatConverter(format_converter.GetAddressOf());
    if (FAILED(hr)) return false;

    hr = format_converter->Initialize(frame.Get(), GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.f, WICBitmapPaletteTypeMedianCut);
    if (FAILED(hr)) return false;

    hr = d2d_viewport->d2d_render_target->CreateBitmapFromWicBitmap(format_converter.Get(), bitmap_.GetAddressOf());
    return SUCCEEDED(hr);
}

RTTR_REGISTRATION
{
    using namespace rttr;
    
    registration::class_<UISprite>("UISprite")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
