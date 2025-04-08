#pragma once
#include "Actor/Actor.h"

namespace UI
{
    class TextBox;
}

class Dummy : public Actor
{
    SHADER_CLASS_HELPER(Dummy)
    GENERATED_BODY(Dummy, Actor)
    
public:
    Dummy(const std::wstring& kName);
    virtual ~Dummy() override = default;

    void Show();
    void Hide();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float delta_time) override;
    
    std::shared_ptr<class CircleColliderComponent> collider_;
    std::shared_ptr<UI::TextBox> text_box_;
    
};
