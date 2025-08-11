#include "pch.h"
#include "UIInventorySlot.h"

#include <CustomPacket.h>

#include "UIInventory.h"
#include "UIItemTooltip.h"
#include "Asset/AssetManager.h"
#include "Subsystems/SessionSubsystem.h"
#include "UI/UIInGameState.h"
#include "UI/UIState.h"
#include "UI/Element/UIImage.h"
#include "Windows/DX/Renderer.h"
#include "Windows/DX/UISprite.h"

UIInventorySlot::UIInventorySlot(const std::wstring& name) :
    UIContainer(name),
    ui_inventory_(nullptr),
    tooltip_(nullptr),
    icon_(nullptr),
    count_text_(nullptr),
    slot_id_(0),
    item_id_(0),
    last_time_(0.f)
{
    SetSize({ 32.f, 32.f });

    UISprite* panel_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\Panel.png");
    
    background_ = AddChild<UIImage>(UIImage::StaticClass(), L"Background");
    background_->SetSprite(panel_sprite, L"Panel_0");
    background_->SetDrawMode(UIImage::DrawMode::kSliced);
    background_->SetIgnoreRayCast(true);
    
    icon_ = AddChild<UIImage>(UIImage::StaticClass(), L"Icon");
    icon_->SetSize(GetSize());
    icon_->SetIgnoreRayCast(true);
    
    count_text_ = AddChild<UIText>(UIText::StaticClass(), L"Count");
    count_text_->SetSize(GetSize());
    count_text_->SetColor(Math::Color::White);
    count_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
    count_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
    count_text_->SetIgnoreRayCast(true);
    count_text_->SetActive(false);
}

void UIInventorySlot::UpdateSlot(uint32_t item_id, uint32_t count)
{
    item_id_ = item_id;
    if (item_id > 0)
    {
        UISprite* ui_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\Item\\" + std::to_wstring(item_id) + L".png");
        if (ui_sprite) icon_->SetSprite(ui_sprite);

        count_text_->SetText(std::to_wstring(count));
        count_text_->SetActive(true);
        return;
    }

    count_text_->SetActive(false);
}

void UIInventorySlot::ResetSlot()
{
    item_id_ = 0;
    
    // icon_->SetRelativePosition(Math::Vector2::Zero());
    // count_text_->SetRelativePosition(Math::Vector2::Zero());
    
    icon_->SetSprite(nullptr, L"");
    count_text_->SetText(L"");
}

void UIInventorySlot::Init()
{
    background_->SetSize(GetSize());
    
    UIContainer::Init();
}

UI::MouseEventResult UIInventorySlot::OnMouseButton(const Math::Vector2& position, MouseButton button, bool is_pressed, double timestamp)
{
    UI::MouseEventResult result = UIContainer::OnMouseButton(position, button, is_pressed, timestamp);
    if (button != MouseButton::kLeft || !is_pressed) return result;
    if (item_id_ == 0) return result;

    result.is_handled = true;
    if (timestamp - last_time_ < .2f)
    {
        Logger::Print(L"Double click!");
        last_time_ = 0.f;
        return result;
    }
    
    last_time_ = timestamp;
    return result;
}

UI::MouseEventResult UIInventorySlot::OnMouseMotion(const Math::Vector2& position, const Math::Vector2& delta)
{
    if (!tooltip_) return { false, UI::CursorState::kIdle };

    EngineSettings* settings = EngineSettings::Get();
    
    Math::Vector2 tooltip_size = tooltip_->GetSize();
    Math::Vector2 tooltip_position = position + Math::Vector2::Up() * 32.f;

    int32_t screen_width = settings->GetScreenWidth();
    int32_t screen_height = settings->GetScreenHeight();

    int32_t overflow_width = tooltip_position.x + tooltip_size.x - screen_width;
    int32_t overflow_height = tooltip_position.y + tooltip_size.y - screen_height;

    if (overflow_width > 0) tooltip_position.x -= overflow_width;
    if (overflow_height > 0) tooltip_position.y -= overflow_height;

    tooltip_->SetAbsolutePosition(tooltip_position);
    tooltip_->Set(item_id_);
    
    return { true, UI::CursorState::kIdle };
}

bool UIInventorySlot::OnMouseEnter()
{
    if (item_id_ == 0) return false;

    auto* state = dynamic_cast<UIInGameState*>(UI::Get()->GetState());
    if (!state) return false;

    tooltip_ = state->GetItemTooltip();
    tooltip_->SetActive(true);
    
    return true;
}

bool UIInventorySlot::OnMouseLeave()
{
    if (item_id_ == 0 || !tooltip_) return false;
    
    tooltip_->SetActive(false);
    tooltip_ = nullptr;
    
    return true;
}

bool UIInventorySlot::OnDragBegin(const Math::Vector2& position)
{
    if (item_id_ == 0) return false;
    if (tooltip_)
    {
        tooltip_->SetActive(false);
        tooltip_ = nullptr;
    }

    UIImage* dragging_item = ui_inventory_->dragging_item_;
    dragging_item->SetActive(true);
    dragging_item->SetSprite(icon_->GetSprite(), icon_->GetFrameIndex());
    
    return true;
}

bool UIInventorySlot::OnDrag(const Math::Vector2& position, const Math::Vector2& delta)
{
    if (item_id_ == 0) return false;

    UIImage* dragging_item = ui_inventory_->dragging_item_;
    dragging_item->SetAbsolutePosition(position - (dragging_item->GetSize() * .5f));

    Math::Color color = icon_->GetColor();
    color.a = 128;
    icon_->SetColor(color);
    
    // icon_->SetAbsolutePosition(position - (icon_->GetSize() * .5f));
    // count_text_->SetAbsolutePosition(position - (count_text_->GetSize() * .5f));
    return true;
}

bool UIInventorySlot::OnDragEnd(const Math::Vector2& position)
{
    if (item_id_ == 0) return false;
    
    Math::Color color = icon_->GetColor();
    color.a = 255;
    icon_->SetColor(color);

    UIImage* dragging_item = ui_inventory_->dragging_item_;
    dragging_item->SetSprite(nullptr, L"");
    dragging_item->SetActive(false);
    
    // icon_->SetRelativePosition(Math::Vector2::Zero());
    // count_text_->SetRelativePosition(Math::Vector2::Zero());

    UIElement* element = UI::Get()->GetState()->RayCast(position);
    if (!element)
    {
        Inventory::Type type = ui_inventory_->tab_;
        int32_t count = ui_inventory_->inventory_->GetItemCount(type, slot_id_);

        if (count == 1)
        {
            DropItemRequest request;
            request.inventory_type = static_cast<uint8_t>(type);
            request.slot_index = slot_id_;
            request.count = 1;
            SessionSubsystem::Get()->SendPacket(request);
        }
        else
        {
            UIPopup::PopupParam param;
            param.caption = L"몇 개나 버리시겠습니까?";
            param.option = UIPopup::PopupOption::OK | UIPopup::PopupOption::Cancel | UIPopup::PopupOption::Edit;
            param.default_input_text = std::to_wstring(count);
            param.content_type = UIEditableText::ContentType::kIntegerNumber;
            param.input_limit = 10;
            param.callback = [&](const std::wstring& input_text, UIPopup::PopupOption option)
            {
                if (option == UIPopup::PopupOption::OK)
                {
                    UIPopup::PopupParam temp_param;
                    temp_param.option = UIPopup::PopupOption::OK;
                    
                    if (input_text == L"" || std::stoll(input_text) <= 0)
                    {
                        temp_param.caption = L"1 이상의 숫자만 가능합니다.";
                        UIPopup::ShowPopup(temp_param);
                        return false;
                    }
                    
                    Inventory::Type temp_type = ui_inventory_->tab_;
                    int32_t temp_count = ui_inventory_->inventory_->GetItemCount(temp_type, slot_id_);
                    if (std::stoll(input_text) > temp_count)
                    {
                        temp_param.caption = std::to_wstring(temp_count) + L" 이하의 숫자만 가능합니다.";
                        UIPopup::ShowPopup(temp_param);
                        return false;
                    }
                    
                    DropItemRequest request;
                    request.inventory_type = static_cast<uint8_t>(temp_type);
                    request.slot_index = slot_id_;
                    request.count = std::stoi(input_text);
                    SessionSubsystem::Get()->SendPacket(request);
                    
                    return true;
                }
                return true;
            };
            UIPopup::ShowPopup(param);
        }
    }
    
    return true;
}

bool UIInventorySlot::OnDrop(const Math::Vector2& position, UIElement* target)
{
    UIInventorySlot* target_slot = dynamic_cast<UIInventorySlot*>(target);
    if (!target_slot) return false;

    uint8_t inventory_type = static_cast<uint8_t>(ui_inventory_->tab_);

    MoveItemRequest request;
    request.inventory_type = inventory_type;
    request.first_slot = target_slot->GetSlotID();
    request.second_slot = slot_id_;
    SessionSubsystem::Get()->SendPacket(request);
    
    return true;
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIInventorySlot>("UIInventorySlot")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
