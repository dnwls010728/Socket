#pragma once

class UIElement;
class UISprite;
class UIState;

class UI : public Singleton<UI>
{
public:
    enum class CursorState : uint8_t
    {
        kIdle = 0
    };

    struct MouseEventResult
    {
        bool is_handled;
        CursorState cursor_state;
    };
    
    UI();
    virtual ~UI() override = default;

    void ChangeState(const rttr::type& type);

    bool IsFocused() const;
    bool IsEditingText() const;

    FORCEINLINE UIState* GetState() const { return state_.get(); }

private:
    friend class Core;
    friend class GameEngine;

    void Init();
    void Tick(float delta_time);
    void Render();
    void OnEvent(const Event& event);

    CursorState cursor_state_;
    
    std::unordered_map<CursorState, UISprite*> cursor_sprites_;

    Math::Vector2 last_position_;
    Math::Vector2 cursor_position_;

    std::unique_ptr<UIState> state_;
    
};
