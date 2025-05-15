#include "pch.h"
#include "UI.h"

#include "UIState.h"
#include "Asset/AssetManager.h"
#include "Event/Events.h"
#include "Windows/DX/UITexture.h"

UI::UI() :
    cursor_state_(CursorState::kIdle),
    cursor_textures_(),
    cursor_position_(Math::Vector2::Zero()),
    state_(nullptr)
{
}

void UI::ChangeState(const rttr::type& type)
{
    rttr::variant var = type.create();
    if (var.is_valid())
    {
        UIState* new_state = var.get_value<UIState*>();
        state_ = std::unique_ptr<UIState>(new_state);
    }
}

void UI::Init()
{
    cursor_textures_[CursorState::kIdle] = AssetManager::Get()->Load<UITexture>(L"UI\\Cursor\\pointer_a.png");
    
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
    WindowsWindow* window = World::Get()->GetWindow();

    if (cursor_textures_[cursor_state_])
    {
        Math::Rect cursor_rect = {
            cursor_position_.x,
            cursor_position_.y,
            static_cast<float>(cursor_textures_[cursor_state_]->GetWidth()),
            static_cast<float>(cursor_textures_[cursor_state_]->GetHeight())
        };
        
        renderer->DrawBitmap(window, cursor_textures_[cursor_state_]->GetTexture(), cursor_rect, Math::Vector2::Zero(), 0.f);
    }
}

void UI::OnEvent(const Event& event)
{
    if (state_) state_->OnEvent(event);

    const uint32_t& type = event.type;
    if (type == static_cast<uint32_t>(EventType::kMouseMotion))
    {
        const Math::Vector2 position = {event.motion.x, event.motion.y};
        cursor_position_ = position;
    }
}
