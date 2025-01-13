#pragma once
#include <vector>

#include "Singleton.h"
#include "Math/Vector2.h"

union Event;

class Widget_OLD;

class Canvas_OLD : public Singleton<Canvas_OLD>
{
public:
    Canvas_OLD();
    virtual ~Canvas_OLD() override = default;

    template<std::derived_from<Widget_OLD> T>
    T* AddWidget(const std::wstring& kName);

    Widget_OLD* FindWidget(const std::wstring& kName);

    template<std::derived_from<Widget_OLD> T>
    T* FindWidget(const std::wstring& kName);

    void GetWidgets(std::vector<Widget_OLD*>& widgets) const;
    void SetWidgetFocus(Widget_OLD* widget);

    float GetScaleRatio() const;

    FORCEINLINE void SetRootWidget(Widget_OLD* widget) { root_widget_ = widget; }
    FORCEINLINE Widget_OLD* GetRootWidget() const { return root_widget_; }

private:
    friend class Core;
    friend class GameEngine;
    friend class Keyboard;
    friend class World;
    friend class Widget_OLD;
    
    Widget_OLD* RayCast(Widget_OLD* widget, const Math::Vector2& kPoint);
    Widget_OLD* FindWidgetParentOfType(Widget_OLD* widget, const rttr::type& kType);
    
    bool IsWidgetType(Widget_OLD* widget, const rttr::type& kType);
    
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

    std::vector<std::shared_ptr<Widget_OLD>> widgets_;
    std::vector<Widget_OLD*> focus_widgets_;

    Widget_OLD* root_widget_;
    Widget_OLD* dragging_widget_;

    bool is_dragging_;
    bool has_begun_drag_;

    Math::Vector2 mouse_position_;
    
};

template <std::derived_from<Widget_OLD> T>
T* Canvas_OLD::AddWidget(const std::wstring& kName)
{
    std::shared_ptr<Widget_OLD> widget = std::make_shared<T>(kName);
    
    widgets_.push_back(widget);

    return static_cast<T*>(widget.get());
}

template <std::derived_from<Widget_OLD> T>
T* Canvas_OLD::FindWidget(const std::wstring& kName)
{
    static_cast<T*>(FindWidget(kName));
}
