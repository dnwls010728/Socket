#include "pch.h"
#include "ChatBalloon.h"

#include <numeric>

#include "Asset/AssetManager.h"
#include "Math/Color.h"
#include "Math/Math.h"
#include "Windows/DX/UISprite.h"

UI_OLD::ChatBalloon::ChatBalloon(const std::wstring& name) :
    Widget(name),
    advances_(),
    total_advance_(0.f)
{
    AssetManager* asset_manager = AssetManager::Get();

    arrow_sprite_ = asset_manager->Load<UISprite>(L"UI\\ChatBalloon_Arrow.png");
    body_sprite_ = asset_manager->Load<UISprite>(L"UI\\ChatBalloon_Body.png");

    SetText(L"Text");
    
}

void UI_OLD::ChatBalloon::SetText(const std::wstring& text)
{
    text_ = text;

    Renderer* renderer = Renderer::Get();
    renderer->GetTextAdvances(text_, L"NanumBarunGothic", 12.f, advances_);

    total_advance_ = std::accumulate(advances_.begin(), advances_.end(), 0.f);
}

std::shared_ptr<UI_OLD::ChatBalloon> UI_OLD::ChatBalloon::Create(const std::wstring& name)
{
    return std::make_shared<ChatBalloon>(name);
}

void UI_OLD::ChatBalloon::Render(Renderer* renderer, WindowsWindow* window)
{
    Widget::Render(renderer, window);
    
    const Math::Rect rect = GetRect();
    const Math::Vector2 pivot_position = GetPivotPosition();
    
    renderer->DrawBitmap(window, arrow_sprite_->GetTexture(), rect, pivot_position);
    
    Math::Rect text_rect = GetRect(
        {pivot_position.x, pivot_position.y - 13.f},
        {total_advance_ + 1.f, 32.f},
        {.5f, .5f}
    );

    Math::Rect body_rect = GetRect(
        {pivot_position.x, pivot_position.y - 13.f},
        {text_rect.width + 32.f, 32.f},
        {.5f, .5f}
    );

    renderer->DrawBitmap(window, body_sprite_->GetTexture(), body_rect, GetPivotPosition(body_rect), 0.f, true, {11.f, 11.f, 21.f, 21.f});
    renderer->DrawString(window, text_, text_rect, GetPivotPosition(text_rect), Math::Color::Black, 0.f, L"NanumBarunGothic", 12.f, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UI_OLD::ChatBalloon>("UI::ChatBalloon")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
