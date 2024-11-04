#pragma once
#include "Actor/Actor.h"

class Editor : public Actor
{
    SHADER_CLASS_HELPER(Editor)
    GENERATED_BODY(Editor, Actor)
    
public:
    Editor(const std::wstring& kName);
    virtual ~Editor() override = default;

protected:
    virtual void Tick(float delta_time) override;
    
};
