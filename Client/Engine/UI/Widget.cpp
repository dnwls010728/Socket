#include "pch.h"
#include "Widget.h"

#include "Canvas.h"

UI::Widget::Widget() :
    parent_(nullptr),
    children_()
{
}

void UI::Widget::AttachCanvas()
{
    Canvas::Get()->AttachWidget(GetSharedThis());
}

void UI::Widget::DetachCanvas()
{
    Canvas::Get()->DetachWidget(GetSharedThis());
}

void UI::Widget::AttachParent(Widget* parent)
{
    if (parent_) DetachParent();
    
    parent_ = parent;
    if (parent_) parent_->children_.push_back(GetSharedThis());
}

void UI::Widget::DetachParent()
{
    if (!parent_) return;

    std::erase(parent_->children_, GetSharedThis());
    parent_ = nullptr;
}

void UI::Widget::Render()
{
    for (const auto& child : children_)
    {
        child->Render();
    }
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UI::Widget>("Widget")
        .constructor<>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
