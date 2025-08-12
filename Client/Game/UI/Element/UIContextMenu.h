#pragma once
#include "UI/UIContainer.h"
#include "Misc/Function.h"
#include <vector>

class UIButton;

class UIContextMenu : public UIContainer
{
    GENERATED_BODY(UIContextMenu, UIContainer)
public:
    using Callback = Function<void(void)>;

    UIContextMenu(const std::wstring& name);
    virtual ~UIContextMenu() override = default;

    void Clear();
    void AddItem(const std::wstring& text, Callback callback);
    void Show(const Math::Vector2& position);
    void Hide();

protected:
    virtual void Init() override;
    virtual void Render() override;
    virtual UI::MouseEventResult OnMouseButton(const Math::Vector2& position, MouseButton button, bool is_pressed, double timestamp) override;

private:
    std::vector<UIButton*> items_;
    std::vector<Callback> callbacks_;
};