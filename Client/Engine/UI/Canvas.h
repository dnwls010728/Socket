#pragma once
#include <vector>

#include "Singleton.h"

union Event;

class Widget;

class Canvas : public Singleton<Canvas>
{
public:
    Canvas();
    virtual ~Canvas() override = default;

    template<std::derived_from<Widget> T>
    T* AddWidget(const std::wstring& kName);

    float GetScaleRatio() const;

private:
    friend class Core;
    friend class GameEngine;
    friend class Keyboard;
    friend class World;
    friend class Widget;
    
    void OnResize(Type::uint32 width, Type::uint32 height);
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
    
};

template <std::derived_from<Widget> T>
T* Canvas::AddWidget(const std::wstring& kName)
{
    std::shared_ptr<Widget> widget = std::make_shared<T>(kName);
    widgets_.push_back(widget);

    return static_cast<T*>(widget.get());
}
