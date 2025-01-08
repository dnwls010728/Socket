#pragma once
#include "Misc/EngineMacros.h"
#include "rttr/registration_friend.h"

class UIWidget : public std::enable_shared_from_this<UIWidget>
{
    SHADER_CLASS_HELPER(UIWidget)
    GENERATED_BODY(UIWidget)

public:
    UIWidget();
    virtual ~UIWidget() = default;

protected:
    virtual void Render();
    
};
