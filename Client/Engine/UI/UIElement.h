#pragma once
#include "Misc/EngineMacros.h"
#include "rttr/registration_friend.h"

class UIElement : public std::enable_shared_from_this<UIElement>
{
    SHADER_CLASS_HELPER(UIElement)
    GENERATED_BODY(UIElement)

public:
    UIElement();
    virtual ~UIElement() = default;

protected:
    virtual void Render();
    
};
