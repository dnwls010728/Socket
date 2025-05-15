#pragma once

class UI : public Singleton<UI>
{
public:
    UI();
    virtual ~UI() override = default;

private:
    friend class GameEngine;

    void Init();
    void Tick(float delta_time);
    void Render();
    
};
