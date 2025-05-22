#include "pch.h"
#include "UIScrollBox.h"

#include "Math/Math.h"
#include "Windows/DX/Renderer.h"

UIScrollBox::UIScrollBox(const std::wstring& name) :
    UIContainer(name)
{
}

void UIScrollBox::SetScrollY(float scroll_y)
{
    static float height = 0.f;
    static bool b = false;

    if (!b)
    {
        
        for (const auto& child : children_)
        {
            height = Math::Max(height, child->GetRelativePosition().y + child->GetSize().y);
        }

        b = true;
    }

    for (auto& child : children_)
    {
        child->SetRelativePosition({
            child->GetRelativePosition().x,
            child->GetRelativePosition().y + ((height - size_.y) * scroll_y_)
        });
    }

    scroll_y_ = scroll_y;

    for (auto& child : children_)
    {
        child->SetRelativePosition({
            child->GetRelativePosition().x,
            child->GetRelativePosition().y - ((height - size_.y) * scroll_y_)
        });
    }
}

void UIScrollBox::Render()
{
    Renderer* renderer = Renderer::Get();

    renderer->BeginLayer(GetAbsolutePosition(), size_);
    UIContainer::Render();
    renderer->EndLayer();
    
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIScrollBox>("UIScrollBox")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
