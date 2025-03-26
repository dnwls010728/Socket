#include "pch.h"
#include "TestWidget.h"

#include "Asset/AssetManager.h"
#include "Windows/DX/Renderer.h"
#include "Windows/DX/UITexture.h"

UI::TestWidget::TestWidget(const std::wstring& kName) :
    Widget(kName)
{
    texture_ = AssetManager::Get()->Load<UITexture>(L"Sprites\\UI\\UISprite.png");
    texture_->SetSlice9Rect({10.f, 10.f, 22.f, 22.f});
}

std::shared_ptr<UI::TestWidget> UI::TestWidget::Create(const std::wstring& kName)
{
    return std::make_shared<TestWidget>(kName);
}

void UI::TestWidget::Render(Renderer* renderer, WindowsWindow* window)
{
    Widget::Render(renderer, window);

    // renderer->DrawBox(window, {position_.x, position_.y, size_.x, size_.y}, size_ * .5f, Math::Color::Black);
    renderer->DrawBitmap(window, texture_->GetTexture(), {position_.x, position_.y, size_.x, size_.y}, size_ * .5f, 0.f, true, texture_->GetSlice9Rect());
}

bool UI::TestWidget::OnMouseEnter()
{
    Logger::Print(L"Widget %s consumed mouse enter event", GetName().c_str());
    return true;
}

bool UI::TestWidget::OnMouseLeave()
{
    Logger::Print(L"Widget %s consumed mouse leave event", GetName().c_str());
    return true;
}

bool UI::TestWidget::OnMouseMotion(const Math::Vector2& kPosition, const Math::Vector2& kDelta)
{
    Logger::Print(L"Widget %s consumed mouse motion event", GetName().c_str());
    return true;
}

bool UI::TestWidget::OnMouseButton(const Math::Vector2& kPosition, MouseButton button, bool is_pressed)
{
    Logger::Print(L"Widget %s consumed mouse button event", GetName().c_str());
    return true;
}

bool UI::TestWidget::OnScroll(const Math::Vector2& kPosition, const Math::Vector2& kDelta)
{
    Logger::Print(L"Widget %s consumed scroll event", GetName().c_str());
    return true;
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UI::TestWidget>("UI::TestWidget")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
