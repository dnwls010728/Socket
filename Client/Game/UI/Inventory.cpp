#include "pch.h"
#include "Inventory.h"

#include "GameInstance.h"
#include "Asset/AssetManager.h"
#include "Inventory/InventoryData.h"
#include "Math/Color.h"
#include "Subsystems/SessionSubsystem.h"
#include "Windows/DX/UITexture.h"

UI::Inventory::Inventory(const std::wstring& name) :
    Widget(name),
    slot_row_(5),
    slot_col_(4),
    max_slots_(0),
    inventory_data_(nullptr),
    dragged_slot_(-1),
    is_dragging_(false)
{
}

std::shared_ptr<UI::Inventory> UI::Inventory::Create(const std::wstring& name)
{
    return std::make_shared<UI::Inventory>(name);
}

void UI::Inventory::Render(Renderer* renderer, WindowsWindow* window)
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

    float slot_start_x = rect.x + 4.f;
    float slot_start_y = rect.y + 24.f;

    for (int16_t row = 0; row < slot_row_; ++row)
    {
        for (int16_t col = 0; col < slot_col_; ++col)
        {
            float slot_x = slot_start_x + col * 36.f;
            float slot_y = slot_start_y + row * 36.f;

            Math::Rect slot_rect(slot_x, slot_y, 32.f, 32.f);
            renderer->DrawBox(window, slot_rect, pivot_position, Math::Color::White);
        }
    }
    
    for (int16_t i = 0; i < max_slots_; ++i)
    {
        int32_t item_id = inventory_data_->GetItemID(i);
        if (item_id >= 0)
        {
            UITexture* texture = AssetManager::Get()->Load<UITexture>(L"UI\\Item\\" + std::to_wstring(item_id) + L".png");
            if (texture)
            {
                Math::Rect icon_rect = GetRect(
                    {slot_start_x + (i % slot_col_) * 36.f, slot_start_y + (i / slot_col_) * 36.f},
                    {32.f, 32.f},
                    {0.f, 1.f}
                );

                renderer->DrawBitmap(window, texture->GetTexture(), icon_rect, GetPivotPosition(icon_rect, {0.f, 1.f}));

                int16_t count = inventory_data_->GetItemCount(i);
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

    Math::Rect color_rect = GetRect(
        { rect.XMin() + 4.f, rect.YMax() - 20.f },
        { rect.width - 8.f, 20.f },
        { 0.f, 1.f }
    );

    std::wstring color = std::to_wstring(inventory_data_->GetColor());
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
}

void UI::Inventory::OnAdd()
{
    Widget::OnAdd();

    SetSize({148.f, 224.f});
    SetPivot({0.f, 1.f});

    max_slots_ = slot_row_ * slot_col_;

    inventory_data_ = GET_SESSION()->GetInventoryData();
}

bool UI::Inventory::OnDragBegin(const Math::Vector2& position)
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
    
    return false;
}

bool UI::Inventory::OnDrag(const Math::Vector2& position, const Math::Vector2& delta)
{
    if (is_dragging_)
    {
        position_ += delta;
        return true;
    }
    
    return false;
}

bool UI::Inventory::OnDragEnd(const Math::Vector2& position)
{
    if (is_dragging_)
    {
        is_dragging_ = false;
        return true;
    }
    
    return false;
}

uint16_t UI::Inventory::GetSlotByPosition(const Math::Vector2& position)
{
    Math::Vector2 relative_position = position - GetRect().Min();

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

    registration::class_<UI::Inventory>("UI::Inventory")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
