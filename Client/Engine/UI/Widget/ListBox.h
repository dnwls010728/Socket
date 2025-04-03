#pragma once
#include "UI/Widget.h"

namespace UI
{
    class ListBox : public Widget
    {
        SHADER_CLASS_HELPER(ListBox)
        GENERATED_BODY(ListBox, Widget)

    protected:
        struct Item
        {
            std::wstring name;
            uint64_t user_data;
        };
        
    public:
        ListBox(const std::wstring& kName);
        virtual ~ListBox() override = default;

        template<typename F, typename = std::enable_if_t<!std::is_same_v<Function<void(Type::uint64)>, std::decay_t<F>>>>
        void OnSelect(F&& func);

        template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
        void OnSelect(M* target, void(M::*func)(Type::uint64));

        template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
        void OnSelect(M* target, void(M::*func)(Type::uint64) const);

        void OnSelect(void(*func)(Type::uint64));

        template<typename F, typename = std::enable_if_t<!std::is_same_v<Function<void(Type::uint64)>, std::decay_t<F>>>>
        void OnDoubleClick(F&& func);

        template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
        void OnDoubleClick(M* target, void(M::*func)(Type::uint64));

        template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
        void OnDoubleClick(M* target, void(M::*func)(Type::uint64) const);

        void OnDoubleClick(void(*func)(Type::uint64));

        void AddItem(const std::wstring& kName, Type::uint64 user_data = 0);
        void RemoveItem(int index);
        void ClearItems();

        FORCEINLINE const std::vector<Item>& GetItems() const { return items_; }

        FORCEINLINE int GetHoveredIndex() const { return hovered_index_; }
        FORCEINLINE int GetSelectedIndex() const { return selected_index_; }

        static std::shared_ptr<ListBox> Create(const std::wstring& kName);

    protected:
        virtual void Tick(float delta_time) override;
        virtual void Render(Renderer* renderer, WindowsWindow* window) override;

        virtual bool OnMouseEnter() override;
        virtual bool OnMouseLeave() override;
        virtual bool OnMouseMotion(const Math::Vector2& kPosition, const Math::Vector2& kDelta) override;
        virtual bool OnMouseButton(const Math::Vector2& kPosition, MouseButton button, bool is_pressed, double timestamp) override;
        virtual bool OnDragBegin(const Math::Vector2& kPosition) override;
        virtual bool OnDrag(const Math::Vector2& kPosition, const Math::Vector2& kDelta) override;
        virtual bool OnDragEnd(const Math::Vector2& kPosition) override;
        virtual bool OnScroll(const Math::Vector2& kPosition, const Math::Vector2& kDelta) override;

        Function<void(Type::uint64)> select_event_;
        Function<void(Type::uint64)> double_click_event_;

        std::vector<Item> items_;

        bool is_hovered_;
        
        int selected_index_;
        int hovered_index_;

        float scroll_offset_y_;
        float min_allowed_scroll_offset_y_;
    
    };

    template <typename F, typename>
    void ListBox::OnSelect(F&& func)
    {
        select_event_ = std::forward<F>(func);
    }

    template <typename M, typename>
    void ListBox::OnSelect(M* target, void(M::* func)(Type::uint64))
    {
        select_event_ = { target, func };
    }

    template <typename M, typename>
    void ListBox::OnSelect(M* target, void(M::* func)(Type::uint64) const)
    {
        select_event_ = { target, func };
    }

    template <typename F, typename>
    void ListBox::OnDoubleClick(F&& func)
    {
        double_click_event_ = std::forward<F>(func);
    }

    template <typename M, typename>
    void ListBox::OnDoubleClick(M* target, void(M::* func)(Type::uint64))
    {
        double_click_event_ = { target, func };
    }

    template <typename M, typename>
    void ListBox::OnDoubleClick(M* target, void(M::* func)(Type::uint64) const)
    {
        double_click_event_ = { target, func };
    }
}
