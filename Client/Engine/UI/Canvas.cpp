#include "pch.h"
#include "Canvas.h"

#include "Logger.h"
#include "Widget.h"
#include "imgui/imgui.h"
#include "Widget/ScrollBox.h"

Canvas::Canvas() :
    width_(0.f),
    height_(0.f),
    reference_resolution_width_(ProjectSettings::kCanvasReferenceWidth),
    reference_resolution_height_(ProjectSettings::kCanvasReferenceHeight),
    match_mode_(ProjectSettings::kMatchMode),
    widgets_(),
    focus_widgets_(),
    root_widget_(nullptr),
    dragging_widget_(nullptr),
    is_dragging_(false),
    has_begun_drag_(false),
    mouse_position_(Math::Vector2::Zero())
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

void Canvas::SetWidgetFocus(Widget* widget)
{
    for (const auto& focus_widget : focus_widgets_)
    {
        if (!focus_widget->IsFocused()) continue;
        focus_widget->OnFocus(false);
    }

    focus_widgets_.clear();

    while (widget)
    {
        focus_widgets_.push_back(widget);
        widget = widget->GetParent();
    }

    for (auto it = focus_widgets_.rbegin(); it != focus_widgets_.rend(); ++it)
    {
        (*it)->OnFocus(true);
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
        if (child->HitTest(kPoint)) return RayCast(child, kPoint);
    }

    if (widget->HitTest(kPoint) && widget->GetParent() != nullptr) return widget;
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
        const WindowEvent& kWindow = kEvent.window;
        
        width_ = kWindow.data1;
        height_ = kWindow.data2;

        for (const auto& widget : widgets_)
        {
            widget->UpdateRect();
        }
    }
    else if (type == static_cast<Type::uint32>(EventType::kMouseMotion))
    {
        Math::Vector2 mouse_position = {kEvent.motion.x, kEvent.motion.y};
        Math::Vector2 mouse_delta = mouse_position - mouse_position_;

        if (root_widget_)
        {
            bool is_handled = false;
            
            if (is_dragging_)
            {
                if (!has_begun_drag_)
                {
                    is_handled |= dragging_widget_->OnBeginDrag(mouse_position);
                    has_begun_drag_ = true;
                }
                else is_handled |= dragging_widget_->OnDrag(mouse_position, mouse_delta);
            }
            
            if (!is_handled) root_widget_->OnMouseMotion(mouse_position, mouse_delta);
        }
        
        mouse_position_ = mouse_position;
    }
    else if (type & static_cast<Type::uint32>(EventType::kMousePressed | EventType::kMouseReleased))
    {
        const MouseButtonEvent& kButton = kEvent.button;
        const Math::Vector2 mouse_position = {kButton.x, kButton.y};
        
        if (root_widget_)
        {
            Widget* drop_widget = RayCast(root_widget_, {kButton.x, kButton.y});
            if (is_dragging_ && !kButton.is_pressed && drop_widget != dragging_widget_)
            {
                if (drop_widget) drop_widget->OnDrop(mouse_position, dragging_widget_);
            }
            
            if (!is_dragging_ && kButton.is_pressed && kButton.button == MouseButton::kLeft)
            {
                dragging_widget_ = RayCast(root_widget_, {kButton.x, kButton.y});
                is_dragging_ = dragging_widget_ != nullptr;
                if (!is_dragging_) SetWidgetFocus(nullptr);
            }
            else if (is_dragging_ && !kButton.is_pressed && kButton.button == MouseButton::kLeft)
            {
                if (has_begun_drag_)
                {
                    dragging_widget_->OnEndDrag(mouse_position);
                    has_begun_drag_ = false;
                }
                
                is_dragging_ = false;
                dragging_widget_ = nullptr;
            }
            
            root_widget_->OnMouseButton({kButton.x, kButton.y}, kButton.button, kButton.is_pressed);
        }
    }
    else if (type == static_cast<Type::uint32>(EventType::kMouseWheel))
    {
        const MouseWheelEvent& kWheel = kEvent.wheel;
        if (root_widget_)
        {
            root_widget_->OnScroll({kWheel.mouse_x, kWheel.mouse_y}, {kWheel.x, kWheel.y});
        }
    }
    else if (type & static_cast<Type::uint32>(EventType::kKeyPressed | EventType::kKeyReleased))
    {
        if (focus_widgets_.size() > 0)
        {
            for (auto it = focus_widgets_.rbegin(); it != focus_widgets_.rend(); ++it)
            {
                const KeyboardEvent kKey = kEvent.key;
                if ((*it)->IsFocused() && (*it)->OnKey(kKey.key_code, kKey.is_repeat)) break;
            }
        }
    }
    else if (type == static_cast<Type::uint32>(EventType::kText))
    {
        if (focus_widgets_.size() > 0)
        {
            for (auto it = focus_widgets_.rbegin(); it != focus_widgets_.rend(); ++it)
            {
                if ((*it)->IsFocused() && (*it)->OnChar(kEvent.text.character)) break;
            }
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
    if (ImGui::Begin("Property"))
    {
        ImGui::Checkbox("IsDragging", &is_dragging_);
        ImGui::Checkbox("HasBegunDrag", &has_begun_drag_);
    }

    ImGui::End();
    
    if (root_widget_)
    {
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
    mouse_position_ = Math::Vector2::Zero();
    
    root_widget_ = nullptr;
    
    widgets_.clear();
    focus_widgets_.clear();
}
