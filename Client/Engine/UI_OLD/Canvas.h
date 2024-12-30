#pragma once
#include <vector>

#include "Singleton.h"
#include "Math/Vector2.h"

union Event;

class Widget;

class Canvas : public Singleton<Canvas>
{
public:
    Canvas();
    virtual ~Canvas() override = default;

    template<std::derived_from<Widget> T>
    T* AddWidget(const std::wstring& kName);

    Widget* FindWidget(const std::wstring& kName);

    template<std::derived_from<Widget> T>
    T* FindWidget(const std::wstring& kName);

    void GetWidgets(std::vector<Widget*>& widgets) const;
    void SetWidgetFocus(Widget* widget);

    float GetScaleRatio() const;

    FORCEINLINE void SetRootWidget(Widget* widget) { root_widget_ = widget; }
    FORCEINLINE Widget* GetRootWidget() const { return root_widget_; }

private:
    friend class Core;
    friend class GameEngine;
    friend class Keyboard;
    friend class World;
    friend class Widget;
    
    Widget* RayCast(Widget* widget, const Math::Vector2& kPoint);
    Widget* FindWidgetParentOfType(Widget* widget, const rttr::type& kType);
    
    bool IsWidgetType(Widget* widget, const rttr::type& kType);
    
    void OnEvent(const Event& kEvent);
    void BeginPlay();
    void Tick(float delta_time);
    void Render();
    void Clear();

    Type::uint32 width_;
    Type::uint32 height_;
    Type::uint32 reference_resolution_width_;
    Type::uint32 reference_resolution_height_;

    float match_mode_;

    std::vector<std::shared_ptr<Widget>> widgets_;
    std::vector<Widget*> focus_widgets_;

    Widget* root_widget_;
    Widget* dragging_widget_;

    bool is_dragging_;
    bool has_begun_drag_;

    Math::Vector2 mouse_position_;
    
};

template <std::derived_from<Widget> T>
T* Canvas::AddWidget(const std::wstring& kName)
{
    std::shared_ptr<Widget> widget = std::make_shared<T>(kName);
    
    widgets_.push_back(widget);

    return static_cast<T*>(widget.get());
}

template <std::derived_from<Widget> T>
T* Canvas::FindWidget(const std::wstring& kName)
{
    static_cast<T*>(FindWidget(kName));
}
