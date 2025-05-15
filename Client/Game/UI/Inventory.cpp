#include "pch.h"
#include "Inventory.h"

#include "GameInstance.h"
#include "Asset/AssetManager.h"
#include "Data/ItemData.h"
#include "Inventory/InventoryManager.h"
#include "Math/Color.h"
#include "Math/Math.h"
#include "Subsystems/DataSubsystem.h"
#include "Subsystems/SessionSubsystem.h"
#include "Windows/DX/UITexture.h"

UI_OLD::Inventory::Inventory(const std::wstring& name) :
    Widget(name),
    slot_row_(10),
    slot_col_(4),
    max_slots_(0),
    scroll_rect_(Math::Rect::Zero()),
    scroll_offset_y_(0.f),
    min_allowed_scroll_offset_y_(0.f),
    inventory_manager_(nullptr),
    dragged_slot_(-1),
    is_dragging_(false)
{
}

std::shared_ptr<UI_OLD::Inventory> UI_OLD::Inventory::Create(const std::wstring& name)
{
    return std::make_shared<Inventory>(name);
}

void UI_OLD::Inventory::Render(Renderer* renderer, WindowsWindow* window)
{
    Widget::Render(renderer, window);

    Math::Rect rect = GetRect();
    Math::Vector2 pivot_position = GetPivotPosition();

    renderer->DrawSolidBox(window, rect, pivot_position, Math::Color(0, 0, 0, 100));
    renderer->DrawBox(window, rect, pivot_position, Math::Color::White);

    Math::Rect title_rect = GetRect(
        rect.Min(),
        {rect.width, 20.f},
        {0.f, 1.f}
    );

    Math::Vector2 title_pivot_position = GetPivotPosition(title_rect, {0.f, 1.f});

    renderer->DrawString(
        window,
        L"인벤토리",
        title_rect, title_pivot_position,
        Math::Color::White,
        0.f,
        L"NanumBarunGothic", 12.f,
        DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER
    );

    renderer->BeginLayer(scroll_rect_);

    float slot_start_x = rect.x + 4.f;
    float slot_start_y = rect.y + 24.f;

    for (int16_t row = 0; row < slot_row_; ++row)
    {
        for (int16_t col = 0; col < slot_col_; ++col)
        {
            float slot_x = slot_start_x + col * 36.f;
            float slot_y = slot_start_y + row * 36.f + scroll_offset_y_;

            Math::Rect slot_rect(slot_x, slot_y, 32.f, 32.f);
            renderer->DrawBox(window, slot_rect, pivot_position, Math::Color::White);
        }
    }
    
    for (int16_t i = 0; i < max_slots_; ++i)
    {
        int32_t item_id = inventory_manager_->GetItemID(i);
        if (item_id >= 0)
        {
            ItemData* item_data = GET_DATA()->GetItem(item_id);
            if (item_data && item_data->IsValid())
            {
                Math::Rect icon_rect = GetRect(
                    {slot_start_x + (i % slot_col_) * 36.f, slot_start_y + (i / slot_col_) * 36.f},
                    {32.f, 32.f},
                    {0.f, 1.f}
                );

                icon_rect.y += scroll_offset_y_;

                Mouse* mouse = Mouse::Get();
                if (dragged_slot_ == i)
                {
                    Math::Vector2 mouse_position = mouse->GetMousePosition();
                    icon_rect.x = mouse_position.x - icon_rect.width / 2.f;
                    icon_rect.y = mouse_position.y - icon_rect.height / 2.f;
                }

                renderer->DrawBitmap(window, item_data->GetIcon()->GetTexture(), icon_rect, GetPivotPosition(icon_rect, {0.f, 1.f}));

                int16_t count = inventory_manager_->GetItemCount(i);
                renderer->DrawString(
                    window,
                    std::to_wstring(count),
                    icon_rect,
                    GetPivotPosition(icon_rect, {0.f, 1.f}),
                    Math::Color::White,
                    0.f,
                    L"NanumBarunGothic", 12.f,
                    DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_FAR
                );
            }
        }
    }

    renderer->EndLayer();

    Math::Rect color_rect = GetRect(
        { rect.XMin() + 4.f, rect.YMax() - 20.f },
        { rect.width - 8.f, 20.f },
        { 0.f, 1.f }
    );

    std::wstring color = std::to_wstring(inventory_manager_->GetColor());
    for (size_t i = color.size(); i > 3; i -= 3)
    {
        color.insert(i - 3, 1, L',');
    }

    renderer->DrawString(
        window,
        color + L"컬러",
        color_rect, pivot_position,
        Math::Color::White,
        0.f,
        L"NanumBarunGothic", 12.f,
        DWRITE_TEXT_ALIGNMENT_TRAILING, DWRITE_PARAGRAPH_ALIGNMENT_CENTER
    );

    Math::Rect scroll_bar_rect = GetRect(
        { rect.XMax() - 10.f, rect.YMin() + 20.f },
        { 10.f, rect.height - 40.f },
        { 0.f, 1.f }
    );

    renderer->DrawSolidBox(window, scroll_bar_rect, pivot_position, Math::Color::Gray);

    float ratio = scroll_rect_.height / (slot_row_ * 36.f);
    float scroll_thumb_height = Math::Clamp(scroll_rect_.height * ratio, 10.f, scroll_rect_.height);
    
    float scroll_ratio = scroll_offset_y_ / min_allowed_scroll_offset_y_;
    scroll_ratio = Math::IsValid(scroll_ratio) ? scroll_ratio : 0.f;
    
    Math::Rect scroll_thumb_rect = GetRect(
        { scroll_bar_rect.x, scroll_bar_rect.y + scroll_ratio * (scroll_bar_rect.height - scroll_thumb_height) },
        { scroll_bar_rect.width, scroll_thumb_height },
        { 0.f, 1.f }
    );
    
    renderer->DrawSolidBox(window, scroll_thumb_rect, pivot_position, Math::Color(255, 255, 255, 100));
}

void UI_OLD::Inventory::OnAdd()
{
    Widget::OnAdd();

    max_slots_ = slot_row_ * slot_col_;

    Math::Rect rect = GetRect();

    scroll_rect_ = GetRect(
        {rect.XMin() + 4, rect.YMin() + 24.f},
        {rect.width - 8.f, rect.height - 48.f},
        {0.f, 1.f}
    );

    float content_height = slot_row_ * 36.f;
    min_allowed_scroll_offset_y_ = scroll_rect_.height - content_height;

    inventory_manager_ = GET_SESSION()->GetInventoryManager();
}

bool UI_OLD::Inventory::OnDragBegin(const Math::Vector2& position)
{
    Math::Rect rect = GetRect();
    Math::Rect title_rect = GetRect(
        rect.Min(),
        {rect.width, 20.f},
        {0.f, 1.f}
    );

    if (Math::Rect::Contains(title_rect, position))
    {
        is_dragging_ = true;
        return true;
    }

    int32_t slot_index = GetSlotByPosition(position);
    if (slot_index > -1)
    {
        dragged_slot_ = slot_index;
        return true;
    }
    
    return false;
}

bool UI_OLD::Inventory::OnDrag(const Math::Vector2& position, const Math::Vector2& delta)
{
    if (is_dragging_)
    {
        position_ += delta;
        
        scroll_rect_.x += delta.x;
        scroll_rect_.y += delta.y;
        return true;
    }
    
    return false;
}

bool UI_OLD::Inventory::OnDragEnd(const Math::Vector2& position)
{
    if (is_dragging_)
    {
        is_dragging_ = false;
        return true;
    }

    if (dragged_slot_ > -1)
    {
        int32_t slot_index = GetSlotByPosition(position);
        if (slot_index > -1 && slot_index != dragged_slot_)
        {
            inventory_manager_->Swap(dragged_slot_, slot_index);
            dragged_slot_ = -1;
            return true;
        }

        dragged_slot_ = -1;
        return true;
    }
    
    return false;
}

bool UI_OLD::Inventory::OnScroll(const Math::Vector2& position, const Math::Vector2& delta)
{
    scroll_offset_y_ = Math::Clamp(scroll_offset_y_ + delta.y * 50.f, min_allowed_scroll_offset_y_, 0.f);
    return true;
}

int32_t UI_OLD::Inventory::GetSlotByPosition(const Math::Vector2& position) const
{
    Math::Vector2 relative_position = position - GetRect().Min() + (Math::Vector2::Down() * scroll_offset_y_);

    int16_t x_min = 4;
    int16_t x_max = x_min + (slot_col_ * 36);
    int16_t y_min = 24;
    int16_t y_max = y_min + (slot_row_ * 36);

    if (relative_position.x < x_min || relative_position.x > x_max ||
        relative_position.y < y_min || relative_position.y > y_max)
        return -1;

    int16_t col = (relative_position.x - x_min) / 36;
    int16_t row = (relative_position.y - y_min) / 36;
    return row * slot_col_ + col;
}


RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UI_OLD::Inventory>("UI::Inventory")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
