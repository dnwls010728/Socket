#include "pch.h"
#include "UITexture.h"

#include "Level/World.h"

UITexture::UITexture() :
    bitmap_(nullptr),
    width_(0),
    height_(0),
    slice9_rect_(Math::Rect::Zero())
{
}

bool UITexture::Load(const std::wstring& kPath)
{
    D2DViewport* d2d_viewport = Renderer::Get()->FindD2DViewport(World::Get()->GetWindow());
    if (!d2d_viewport) return false;
    
    Microsoft::WRL::ComPtr<IWICImagingFactory> factory_ = Renderer::Get()->GetImageFactory();

    Microsoft::WRL::ComPtr<IWICBitmapDecoder> decoder;
    HRESULT hr = factory_->CreateDecoderFromFilename(kPath.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, decoder.GetAddressOf());
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
    
    registration::class_<UITexture>("UITexture")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
