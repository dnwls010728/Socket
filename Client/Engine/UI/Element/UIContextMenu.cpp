#include "pch.h"
#include "UIContextMenu.h"

#include "UIButton.h"
#include "Asset/AssetManager.h"
#include "Windows/DX/Renderer.h"
#include "Windows/DX/UISprite.h"

UIContextMenu::UIContextMenu(const std::wstring& name) :
    UIContainer(name)
{
}

void UIContextMenu::Init()
{
    UIContainer::Init();
}

void UIContextMenu::Clear()
{
    for (UIButton* item : items_)
        RemoveChild(item);
    items_.clear();
    callbacks_.clear();
    SetSize({0.f, 0.f});
}

void UIContextMenu::AddItem(const std::wstring& text, Callback callback)
{
    size_t index = items_.size();
    UIButton* button = AddChild<UIButton>(UIButton::StaticClass(), text);
    UISprite* button_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\ButtonSheet.png");
    button->SetSize({120.f, 20.f});
    button->SetText(text);
    button->SetTextColor(Math::Color::White);
    button->SetSprite(UIButton::State::kNormal, button_sprite, L"ButtonSheet_0");
    button->SetSprite(UIButton::State::kHover, button_sprite, L"ButtonSheet_1");
    button->SetSprite(UIButton::State::kPressed, button_sprite, L"ButtonSheet_2");
    button->SetSprite(UIButton::State::kDisabled, button_sprite, L"ButtonSheet_3");
    button->SetDrawMode(UIImage::DrawMode::kSliced);
    button->SetRelativePosition({0.f, 20.f * static_cast<float>(index)});
    button->SetActive(true);
    
    callbacks_.push_back(std::move(callback));
    button->OnClick([this, index]() {
        callbacks_[index]();
        SetActive(false);
    });
    items_.push_back(button);
    SetSize({120.f, 20.f * static_cast<float>(items_.size())});
}

void UIContextMenu::Show(const Math::Vector2& position)
{
    SetAbsolutePosition(position);
    SetActive(true);
}

void UIContextMenu::Hide()
{
    SetActive(false);
}

UI::MouseEventResult UIContextMenu::OnMouseButton(const Math::Vector2& position, MouseButton button, bool is_pressed, double timestamp)
{
    UI::MouseEventResult result = UIContainer::OnMouseButton(position, button, is_pressed, timestamp);
    if (is_pressed && button == MouseButton::kLeft && !IsInRange(position))
    {
        Hide();
        result.is_handled = true;
    }
    return result;
}

void UIContextMenu::Render()
{
    Renderer::Get()->DrawSolidBox(GetAbsolutePosition(), GetSize(), {0, 0, 0, 180});
    UIContainer::Render();
}

RTTR_REGISTRATION
{
    using namespace rttr;
    registration::class_<UIContextMenu>("UIContextMenu")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}