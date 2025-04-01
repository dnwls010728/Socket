#include "pch.h"
#include "ListBox.h"

#include "Math/Color.h"
#include "Math/Math.h"
#include "Windows/DX/Renderer.h"

// TODO: GetRect로 전체 교체 작업 필요
UI::ListBox::ListBox(const std::wstring& kName) :
    Widget(kName),
    select_event_([&](Type::uint64) {}),
    items_(),
    is_hovered_(false),
    selected_index_(-1),
    hovered_index_(-1),
    scroll_offset_y_(0.f),
    min_allowed_scroll_offset_y_(0.f)
{
}

void UI::ListBox::OnSelect(void(* func)(Type::uint64))
{
    select_event_ = func;
}

void UI::ListBox::AddItem(const std::wstring& kName, Type::uint64 user_data)
{
    Item new_item;
    new_item.name = kName;
    new_item.user_data = user_data;
    items_.push_back(new_item);

    float offset_y = 0.f;
    for (Type::uint32 i = 0; i < items_.size(); ++i)
    {
        offset_y += 30.f;
    }

    min_allowed_scroll_offset_y_ = Math::Min(size_.y - offset_y, 0.f);
}

void UI::ListBox::RemoveItem(int index)
{
    if (index < 0 || index >= items_.size())
        return;

    items_.erase(items_.begin() + index);
    
    float offset_y = 0.f;
    for (Type::uint32 i = 0; i < items_.size(); ++i)
    {
        offset_y += 30.f;
    }

    min_allowed_scroll_offset_y_ = Math::Min(size_.y - offset_y, 0.f);

    if (selected_index_ > items_.size() - 1) selected_index_ = items_.size() - 1;
    if (hovered_index_ > items_.size() - 1) hovered_index_ = items_.size() - 1;

    if (items_.empty())
    {
        if (selected_index_ >= 0) selected_index_ = -1;
        if (hovered_index_ >= 0) hovered_index_ = -1;
    }
    else
    {
        select_event_(items_[selected_index_].user_data);
    }
}

void UI::ListBox::ClearItems()
{
    items_.clear();

    min_allowed_scroll_offset_y_ = 0.f;
    selected_index_ = -1;
    hovered_index_ = -1;
}

std::shared_ptr<UI::ListBox> UI::ListBox::Create(const std::wstring& kName)
{
    return std::make_shared<ListBox>(kName);
}

void UI::ListBox::Tick(float delta_time)
{
    Widget::Tick(delta_time);

    float target_scroll_offset_y = scroll_offset_y_;

    if (scroll_offset_y_ > 0.f) target_scroll_offset_y = 0.f;
    else if (scroll_offset_y_ < min_allowed_scroll_offset_y_) target_scroll_offset_y = min_allowed_scroll_offset_y_;

    if (target_scroll_offset_y != scroll_offset_y_)
    {
        scroll_offset_y_ = Math::Lerp(scroll_offset_y_, target_scroll_offset_y, delta_time * 5.f);
    }
}

void UI::ListBox::Render(Renderer* renderer, WindowsWindow* window)
{
    Widget::Render(renderer, window);

    const Math::Rect kRect = GetRect();

    renderer->DrawBox(window, kRect, GetPivotPosition(), Math::Color::Black, 0.f, 1.f);

    renderer->BeginLayer(GetRect());
    for (Type::uint32 i = 0; i < items_.size(); ++i)
    {
        const Item& kItem = items_[i];
        const Math::Rect kItemRect = GetRect(
            {kRect.x, kRect.y + (i * 30.f) + scroll_offset_y_},
            {kRect.width, 30.f},
            {0.f, 1.f}
        );

        renderer->DrawString(window, kItem.name, kItemRect, GetPivotPosition(kItemRect, {0.f, 1.f}), Math::Color::White, 0.f, L"Nanum18", DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    }

    if (selected_index_ >= 0)
    {
        const Math::Rect kItemRect = {
            kRect.x,
            kRect.y + (selected_index_ * 30.f) + scroll_offset_y_,
            kRect.width,
            30.f
        };

        renderer->DrawSolidBox(window, kItemRect, GetPivotPosition(kItemRect, {0.f, 1.f}), {255, 255, 255, 100}, 0.f);
    }
    
    if (hovered_index_ >= 0)
    {
        const Math::Rect kItemRect = {
            kRect.x,
            kRect.y + (hovered_index_ * 30.f) + scroll_offset_y_,
            kRect.width,
            30.f
        };

        renderer->DrawSolidBox(window, kItemRect, GetPivotPosition(kItemRect, {0.f, 1.f}), {255, 255, 255, 100}, 0.f);
    }
    renderer->EndLayer();

    const Math::Rect kScrollBarRect = {
        kRect.x + kRect.width - 10.f,
        kRect.y,
        10.f,
        kRect.height
    };

    renderer->DrawBox(window, kScrollBarRect, GetPivotPosition(kScrollBarRect, {0.f, 1.f}), Math::Color::Gray, 0.f, 1.f);

    renderer->BeginLayer(kScrollBarRect);
    float ratio = kRect.height / (items_.size() * 30.f);
    
    const float scroll_thumb_height = Math::Clamp(kRect.height * ratio, 10.f, kRect.height);
    const Math::Rect kScrollThumbRect = {
        kScrollBarRect.x,
        kScrollBarRect.y + (scroll_offset_y_ / min_allowed_scroll_offset_y_) * (kScrollBarRect.height - scroll_thumb_height),
        kScrollBarRect.width,
        scroll_thumb_height
    };

    renderer->DrawSolidBox(window, kScrollThumbRect, GetPivotPosition(kScrollThumbRect), Math::Color(255, 255, 255, 100), 0.f);
    renderer->EndLayer();
}

bool UI::ListBox::OnMouseEnter()
{
    is_hovered_ = true;
    return true;
}

bool UI::ListBox::OnMouseLeave()
{
    is_hovered_ = false;
    return true;
}

bool UI::ListBox::OnMouseMotion(const Math::Vector2& kPosition, const Math::Vector2& kDelta)
{
    if (items_.empty()) return false;

    const Math::Rect kRect = GetRect();
    for (Type::uint32 i = 0; i < items_.size(); ++i)
    {
        const Math::Rect kItemRect = {
            kRect.x,
            kRect.y + (i * 30.f) + scroll_offset_y_,
            kRect.width,
            30.f
        };

        if (Math::Rect::Contains(kItemRect, kPosition))
        {
            hovered_index_ = i;
            return true;
        }
    }

    return false;
}

bool UI::ListBox::OnMouseButton(const Math::Vector2& kPosition, MouseButton button, bool is_pressed)
{
    Widget::OnMouseButton(kPosition, button, is_pressed);
    if (button == MouseButton::kLeft && is_pressed)
    {
        const Math::Rect kRect = GetRect();
        for (Type::uint32 i = 0; i < items_.size(); ++i)
        {
            const Math::Rect kItemRect = {
                kRect.x,
                kRect.y + (i * 30.f) + scroll_offset_y_,
                kRect.width,
                30.f
            };

            if (Math::Rect::Contains(kItemRect, kPosition))
            {
                const Item& kItem = items_[i];
                select_event_(kItem.user_data);
                selected_index_ = i;
                return true;
            }
        }
    }
    
    return true;
}

bool UI::ListBox::OnDrag(const Math::Vector2& kPosition, const Math::Vector2& kDelta)
{
    if (items_.empty()) return false;
    
    scroll_offset_y_ += kDelta.y;
    return true;
}

bool UI::ListBox::OnScroll(const Math::Vector2& kPosition, const Math::Vector2& kDelta)
{
    if (items_.empty()) return false;
    
    const float scroll_amount = kDelta.y * 10.f;
    scroll_offset_y_ += scroll_amount;
    
    return true;
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UI::ListBox>("UI::ListBox")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
