#include "pch.h"
#include "Image.h"

#include "Asset/AssetManager.h"
#include "Windows/DX/UITexture.h"

UI_OLD::Image::Image(const std::wstring& kName) :
    Widget(kName),
    texture_(nullptr)
{
}

void UI_OLD::Image::SetTexture(const std::wstring& path)
{
    texture_ = AssetManager::Get()->Load<UITexture>(path);
}

std::shared_ptr<UI_OLD::Image> UI_OLD::Image::Create(const std::wstring& name)
{
    return std::make_shared<Image>(name);
}

void UI_OLD::Image::Render(Renderer* renderer, WindowsWindow* window)
{
    Widget::Render(renderer, window);
    if (!texture_) return;

    renderer->DrawBitmap(window, texture_->GetTexture(), GetRect(), GetPivotPosition());
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UI_OLD::Image>("UI::Image")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
