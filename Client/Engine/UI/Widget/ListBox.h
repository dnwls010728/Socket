#pragma once
#include "UI/Widget.h"

namespace UI
{
    class ListBox : public Widget
    {
        SHADER_CLASS_HELPER(ListBox)
        GENERATED_BODY(ListBox, Widget)
        
    public:
        ListBox(const std::wstring& kName);
        virtual ~ListBox() override = default;

        template<typename F, typename = std::enable_if_t<!std::is_same_v<Function<void(uint64_t)>, std::decay_t<F>>>>
        void OnSelect(F&& func);

        template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
        void OnSelect(M* target, void(M::*func)(uint64_t));

        template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
        void OnSelect(M* target, void(M::*func)(uint64_t) const);

        void OnSelect(void(*func)(uint64_t));

        void AddItem(const std::wstring& kName, uint64_t user_data = 0);
        void RemoveItem(int index);

    protected:
        struct Item
        {
            std::wstring name;
            uint64_t user_data;
        };

        virtual void Tick(float delta_time) override;
        virtual void Render(Renderer* renderer, WindowsWindow* window) override;

        virtual bool OnMouseEnter() override;
        virtual bool OnMouseLeave() override;
        virtual bool OnMouseMotion(const Math::Vector2& kPosition, const Math::Vector2& kDelta) override;
        virtual bool OnMouseButton(const Math::Vector2& kPosition, MouseButton button, bool is_pressed) override;
        virtual bool OnScroll(const Math::Vector2& kPosition, const Math::Vector2& kDelta) override;

        Function<void(uint64_t)> select_event_;

        std::vector<Item> items_;

        bool is_hovered_;
        
        int hovered_index_;
        int selected_index_;

        float scroll_offset_y_;
        float min_allowed_scroll_offset_y_;
    
    };

    template <typename F, typename>
    void ListBox::OnSelect(F&& func)
    {
        select_event_ = std::forward<F>(func);
    }

    template <typename M, typename>
    void ListBox::OnSelect(M* target, void(M::* func)(uint64_t))
    {
        select_event_ = { target, func };
    }

    template <typename M, typename>
    void ListBox::OnSelect(M* target, void(M::* func)(uint64_t) const)
    {
        select_event_ = { target, func };
    }
}
