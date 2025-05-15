#pragma once

class UITexture;
class UIState;

class UI : public Singleton<UI>
{
public:
    enum class CursorState : uint8_t
    {
        kIdle = 0
    };
    
    UI();
    virtual ~UI() override = default;

    void ChangeState(const rttr::type& type);

private:
    friend class Core;
    friend class GameEngine;

    void Init();
    void Tick(float delta_time);
    void Render();
    void OnEvent(const Event& event);

    CursorState cursor_state_;
    
    std::unordered_map<CursorState, UITexture*> cursor_textures_;

    Math::Vector2 cursor_position_;

    std::unique_ptr<UIState> state_;
    
};
