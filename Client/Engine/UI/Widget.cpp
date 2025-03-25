#include "pch.h"
#include "Widget.h"

#include "Logger.h"
#include "Input/Keyboard.h"

UI::Widget::Widget(const std::wstring& kName) :
    name_(kName),
    parent_(nullptr),
    children_()
{
}

void UI::Widget::AttachToWidget(Widget* parent)
{
    parent_ = parent;
    parent_->children_.push_back(GetSharedThis());
}

void UI::Widget::DetachFromWidget()
{
    if (!parent_)
    {
        parent_ = nullptr;
        std::erase(parent_->children_, GetSharedThis());
    }
}

std::shared_ptr<UI::Widget> UI::Widget::Create(const std::wstring& kName)
{
    return std::make_shared<Widget>(kName);
}

void UI::Widget::Tick(float delta_time)
{
    // Logger::Print(L"WIDGET TICK: %s", name_.c_str());
    
    for (Type::uint32 i = 0; i < children_.size(); ++i)
    {
        Widget* widget = children_[i].get();
        widget->Tick(delta_time);
    }
}

void UI::Widget::Render()
{
    for (Type::uint32 i = 0; i < children_.size(); ++i)
    {
        Widget* widget = children_[children_.size() - i - 1].get();
        widget->Render();
    }

    // Logger::Print(L"WIDGET RENDER: %s", name_.c_str());
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UI::Widget>("UI::Widget")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
