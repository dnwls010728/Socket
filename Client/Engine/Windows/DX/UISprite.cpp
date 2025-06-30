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
                UISpriteFrame sprite_frame;
                sprite_frame.offset.x = frame["rect"]["x"].as<float>();
                sprite_frame.offset.y = frame["rect"]["y"].as<float>();
                sprite_frame.size.x = frame["rect"]["width"].as<float>();
                sprite_frame.size.y = frame["rect"]["height"].as<float>();
                sprite_frame.border_min.x = frame["border"]["left"].as<float>();
                sprite_frame.border_min.y = frame["border"]["top"].as<float>();
                sprite_frame.border_max.x = frame["border"]["right"].as<float>();
                sprite_frame.border_max.y = frame["border"]["bottom"].as<float>();
                
                std::wstring name = StringHelper::UTF8ToUTF16(frame["name"].as<std::string>());
                frames_[name] = sprite_frame;
            }
        }
    }
    else
    {
        frames_.clear();

        UISpriteFrame frame;
        frame.offset = Math::Vector2::Zero();
        frame.size = { static_cast<float>(width_), static_cast<float>(height_) };
        frame.border_min = Math::Vector2::Zero();
        frame.border_max = Math::Vector2::Zero();

        std::wstring filename = FileHelper::GetFilenameWithoutExtension(path);
        frames_[filename + L"_0"] = frame;
    }
    
    return true;
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
