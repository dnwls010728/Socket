#include "pch.h"
#include "UI.h"

#include "UIState.h"
#include "Asset/AssetManager.h"
#include "Event/Events.h"
#include "Windows/DX/UISprite.h"

UI::UI() :
    cursor_state_(CursorState::kIdle),
    cursor_sprites_(),
    last_position_(Math::Vector2::Zero()),
    cursor_position_(Math::Vector2::Zero()),
    state_(nullptr)
{
}

void UI::ChangeState(const rttr::type& type)
{
    if (state_)
    {
        if (state_->get_type() == type) return;
        state_->Uninit();
    }
    
    rttr::variant var = type.create();
    if (var.is_valid())
    {
        UIState* new_state = var.get_value<UIState*>();
        state_ = std::unique_ptr<UIState>(new_state);
        if (state_) state_->Init();
    }
}

void UI::SetFocus(UIElement* element)
{
    if (!state_) return;
    state_->SetFocus(element);
}

bool UI::IsFocused() const
{
    if (!state_) return false;
    return state_->IsFocused();
}

bool UI::IsEditingText() const
{
    if (!state_) return false;
    return state_->IsEditingText();
}

void UI::Init()
{
    cursor_sprites_[CursorState::kIdle] = AssetManager::Get()->Load<UISprite>(L"UI\\Cursor\\pointer_a.png");
    
    ChangeState(UIState::StaticClass());
}

void UI::Tick(float delta_time)
{
    if (state_) state_->Tick(delta_time);
}

void UI::Render()
{
    if (state_) state_->Render();

    Renderer* renderer = Renderer::Get();

    if (auto* cursor_sprite = cursor_sprites_[cursor_state_])
    {
        Math::Vector2 size = { static_cast<float>(cursor_sprite->GetWidth()), static_cast<float>(cursor_sprite->GetHeight()) };
        renderer->DrawSimpleSprite(cursor_sprite, 0, cursor_position_, size);
    }
}

void UI::OnEvent(const Event& event)
{
    const uint32_t& type = event.type;
    if (type == static_cast<uint32_t>(EventType::kMouseMotion))
    {
        const MouseMotionEvent& motion_event = event.motion;
        
        const Math::Vector2 position = {motion_event.x, motion_event.y};
        const Math::Vector2 delta = position - last_position_;

        if (state_)
        {
            MouseEventResult result = state_->OnMouseMotion(position, delta);
            if (result.is_handled) cursor_state_ = result.cursor_state;
        }

        cursor_position_ = position;
        last_position_ = position;
    }
    else if (type & static_cast<uint32_t>(EventType::kMouseChanged))
    {
        const MouseButtonEvent& button_event = event.button;
        const Math::Vector2 position = {button_event.x, button_event.y};

        if (state_)
        {
            MouseEventResult result = state_->OnMouseButton(position, button_event.button, button_event.is_pressed, button_event.timestamp);
            if (result.is_handled) cursor_state_ = result.cursor_state;
        }
    }
    else if (type == static_cast<uint32_t>(EventType::kMouseWheel))
    {
        const MouseWheelEvent& wheel_event = event.wheel;
        const Math::Vector2 position = {wheel_event.mouse_x, wheel_event.mouse_y};
        const Math::Vector2 delta = {wheel_event.x, wheel_event.y};

        if (state_) state_->OnScroll(position, delta);
    }
    else if (type & static_cast<uint32_t>(EventType::kKeyChanged))
    {
        const KeyboardEvent& key_event = event.key;
        if (state_) state_->OnKey(key_event.scancode, key_event.is_repeat);
    }
    else if (type == static_cast<uint32_t>(EventType::kText))
    {
        const TextEvent& text_event = event.text;
        if (state_) state_->OnChar(text_event.character);
    }
}
