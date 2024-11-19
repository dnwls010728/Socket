#include "pch.h"
#include "Canvas.h"

#include "Logger.h"
#include "Widget.h"
#include "Widget/ScrollBox.h"

Canvas::Canvas() :
    width_(0.f),
    height_(0.f),
    reference_resolution_width_(ProjectSettings::kCanvasReferenceWidth),
    reference_resolution_height_(ProjectSettings::kCanvasReferenceHeight),
    match_mode_(ProjectSettings::kMatchMode),
    widgets_(),
    root_widget_(nullptr),
    hovered_widget_(nullptr),
    focused_widget_(nullptr),
    dragging_widget_(nullptr),
    previous_mouse_position_(Math::Vector2::Zero())
{
}

Widget* Canvas::FindWidget(const std::wstring& kName)
{
    for (const auto& widget : widgets_)
    {
        if (widget->name_ == kName) return widget.get();
    }

    return nullptr;
}

void Canvas::GetWidgets(std::vector<Widget*>& widgets) const
{
    for (const auto& widget : widgets_)
    {
        widgets.push_back(widget.get());
    }
}

float Canvas::GetScaleRatio() const
{
    const float width_ratio = width_ / static_cast<float>(reference_resolution_width_);
    const float height_ratio = height_ / static_cast<float>(reference_resolution_height_);

    return width_ratio * (1.f - match_mode_) + height_ratio * match_mode_;
}

Widget* Canvas::RayCast(Widget* widget, const Math::Vector2& kPoint)
{
    if (!widget) return nullptr;
    for (auto it = widget->children_.rbegin(); it != widget->children_.rend(); ++it)
    {
        Widget* child = *it;
        Widget* result = RayCast(child, kPoint);
        if (result) return result;
    }

    if (widget->is_ray_cast_target_ && widget->HitTest(kPoint)) return widget;
    return nullptr;
}

Widget* Canvas::FindWidgetParentOfType(Widget* widget, const rttr::type& kType)
{
    if (!widget) return nullptr;
    
    Widget* parent = widget->parent_;
    if (!parent) return nullptr;

    rttr::type parent_type = rttr::type::get(*parent);
    if (parent_type.is_derived_from(kType)) return parent;
    return FindWidgetParentOfType(parent, kType);
}

bool Canvas::IsWidgetType(Widget* widget, const rttr::type& kType)
{
    if (!widget) return false;

    rttr::type widget_type = rttr::type::get(*widget);
    return widget_type.is_derived_from(kType);
}

void Canvas::OnEvent(const Event& kEvent)
{
    const Type::uint32& type = kEvent.type;

    if (type == static_cast<Type::uint32>(EventType::kWindowSize))
    {
        width_ = kEvent.window.data1;
        height_ = kEvent.window.data2;

        for (const auto& widget : widgets_)
        {
            widget->UpdateRect();
        }
    }
    else if (type == static_cast<Type::uint32>(EventType::kMousePressed))
    {
        if (kEvent.button.button == MouseButton::kLeft)
        {
            if (focused_widget_ != hovered_widget_)
            {
                if (focused_widget_)
                {
                    focused_widget_->is_focused_ = false;
                    focused_widget_->OnFocusChanged(false);
                }
                
                focused_widget_ = hovered_widget_;
                if (focused_widget_)
                {
                    focused_widget_->is_focused_ = true;
                    focused_widget_->OnFocusChanged(true);
                }
            }
            
            if (hovered_widget_)
            {
                hovered_widget_->OnMousePressed.Execute();
                
                previous_mouse_position_ = {kEvent.button.x, kEvent.button.y};
                
                dragging_widget_ = hovered_widget_;
                dragging_widget_->OnDragStart.Execute(previous_mouse_position_);
            }
        }
    }
    else if (type == static_cast<Type::uint32>(EventType::kMouseReleased))
    {
        if (focused_widget_ && focused_widget_ == hovered_widget_)
        {
            focused_widget_->OnMouseReleased.Execute();
        }
        
        if (dragging_widget_)
        {
            previous_mouse_position_ = {kEvent.button.x, kEvent.button.y};
            
            dragging_widget_->OnDragEnd.Execute(previous_mouse_position_);
            dragging_widget_ = nullptr;

            if (hovered_widget_) hovered_widget_->OnDrop.Execute(previous_mouse_position_);
        }
    }
    else if (type == static_cast<Type::uint32>(EventType::kMouseMotion))
    {
        if (dragging_widget_)
        {
            const Math::Vector2 mouse_position = {kEvent.motion.x, kEvent.motion.y};
            
            Math::Vector2 delta = mouse_position - previous_mouse_position_;
            previous_mouse_position_ = mouse_position;
            
            dragging_widget_->OnDrag.Execute(delta);
        }
    }
    else if (type & static_cast<Type::uint32>(EventType::kKeyPressed | EventType::kKeyReleased))
    {
        if (focused_widget_) focused_widget_->OnInputKey(kEvent.key.key_code, kEvent.key.is_repeat);
    }
    else if (type == static_cast<Type::uint32>(EventType::kText))
    {
        if (focused_widget_) focused_widget_->OnInputText(kEvent.text.character);
    }
    else if (type == static_cast<Type::uint32>(EventType::kMouseWheel))
    {
        Widget* scroll_widget;
        
        if (IsWidgetType(hovered_widget_, ScrollBox::StaticClass())) scroll_widget = hovered_widget_;
        else scroll_widget = FindWidgetParentOfType(hovered_widget_, ScrollBox::StaticClass());

        if (scroll_widget)
        {
            ScrollBox* scroll_box = dynamic_cast<ScrollBox*>(scroll_widget);
            scroll_box->OnScroll(kEvent.wheel.x, kEvent.wheel.y);
        }
    }
}

void Canvas::BeginPlay()
{
    if (root_widget_)
    {
        root_widget_->BeginPlay();
    }
}

void Canvas::Tick(float delta_time)
{
    Mouse* mouse = Mouse::Get();
    Math::Vector2 mouse_position = mouse->GetMousePosition();
    
    if (root_widget_)
    {
        Widget* hovered_widget = RayCast(root_widget_, mouse_position);
        if (hovered_widget != hovered_widget_)
        {
            if (hovered_widget_) hovered_widget_->is_hovered_ = false;
            hovered_widget_ = hovered_widget;
            if (hovered_widget_) hovered_widget_->is_hovered_ = true;
        }
        
        root_widget_->Tick(delta_time);
    }
}

void Canvas::Render()
{
    if (root_widget_)
    {
        root_widget_->Render();
    }
}

void Canvas::Clear()
{
    previous_mouse_position_ = Math::Vector2::Zero();
    
    root_widget_ = nullptr;
    hovered_widget_ = nullptr;
    focused_widget_ = nullptr;
    dragging_widget_ = nullptr;
    
    widgets_.clear();
}
