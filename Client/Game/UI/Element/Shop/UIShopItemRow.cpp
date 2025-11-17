#include "pch.h"
#include "UIShopItemRow.h"

#include "Asset/AssetManager.h"
#include "Math/Color.h"
#include "Subsystems/DataSubsystem.h"
#include "UI/UI.h"
#include "UI/UIInGameState.h"
#include "UI/Element/Inventory/UIItemTooltip.h"
#include "UI/Element/UIImage.h"
#include "UI/Element/UIText.h"
#include "Windows/DX/UISprite.h"

namespace
{
    constexpr float kIconSize = 36.f;
}

UIShopItemRow::UIShopItemRow(const std::wstring& name) :
    UIContainer(name),
    background_(nullptr),
    icon_(nullptr),
    name_text_(nullptr),
    price_text_(nullptr),
    count_text_(nullptr),
    tooltip_(nullptr),
    item_id_(0),
    last_click_time_(0.0),
    double_click_event_([](){})
{
    SetSize({280.f, 46.f});

    UISprite* panel_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\Panel.png");

    background_ = AddChild<UIImage>(UIImage::StaticClass(), L"Background");
    background_->SetSprite(panel_sprite, L"Panel_0");
    background_->SetDrawMode(UIImage::DrawMode::kSliced);
    background_->SetColor(Math::Color(255, 255, 255, 200));
    background_->SetIgnoreRayCast(true);

    icon_ = AddChild<UIImage>(UIImage::StaticClass(), L"Icon");
    icon_->SetRelativePosition({6.f, 6.f});
    icon_->SetSize({kIconSize, kIconSize});
    icon_->SetIgnoreRayCast(true);

    name_text_ = AddChild<UIText>(UIText::StaticClass(), L"Name");
    name_text_->SetRelativePosition({kIconSize + 14.f, 4.f});
    name_text_->SetSize({220.f, 18.f});
    name_text_->SetColor(Math::Color::White);
    name_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    name_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
    name_text_->SetIgnoreRayCast(true);

    price_text_ = AddChild<UIText>(UIText::StaticClass(), L"Price");
    price_text_->SetRelativePosition({kIconSize + 14.f, 22.f});
    price_text_->SetSize({200.f, 18.f});
    price_text_->SetColor(Math::Color(255, 230, 120, 255));
    price_text_->SetIgnoreRayCast(true);

    count_text_ = AddChild<UIText>(UIText::StaticClass(), L"Count");
    count_text_->SetRelativePosition({GetSize().x - 80.f, 4.f});
    count_text_->SetSize({70.f, 18.f});
    count_text_->SetColor(Math::Color::White);
    count_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
    count_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
    count_text_->SetIgnoreRayCast(true);
}

void UIShopItemRow::SetItem(uint32_t item_id)
{
    item_id_ = item_id;
    if (item_id == 0)
    {
        icon_->SetSprite(nullptr, L"");
        name_text_->SetText(L"-");
        tooltip_ = nullptr;
        return;
    }

    const ItemData* item_data = DataSubsystem::Get()->GetItem(item_id);
    if (!item_data)
    {
        name_text_->SetText(L"알 수 없는 아이템");
        icon_->SetSprite(nullptr, L"");
        return;
    }

    name_text_->SetText(item_data->name);
    AssetManager::Get()->LoadAsync<UISprite>(item_data->ui_icon.path, [this, frame_index = item_data->ui_icon.index](UISprite* sprite)
    {
        if (!sprite)
        {
            static UISprite* missing = AssetManager::Get()->Load<UISprite>(L"UI\\Item\\Missing.png");
            sprite = missing;
        }
        icon_->SetSprite(sprite, frame_index);
    });
}

void UIShopItemRow::SetPriceText(const std::wstring& text) const
{
    price_text_->SetActive(!text.empty());
    price_text_->SetText(text);
}

void UIShopItemRow::SetCountText(const std::wstring& text) const
{
    count_text_->SetActive(!text.empty());
    count_text_->SetText(text);
}

void UIShopItemRow::SetDoubleClickHandler(const Function<void(void)>& handler)
{
    double_click_event_ = handler;
}

void UIShopItemRow::Clear()
{
    item_id_ = 0;
    icon_->SetSprite(nullptr, L"");
    name_text_->SetText(L"-");
    price_text_->SetText(L"");
    price_text_->SetActive(false);
    count_text_->SetText(L"");
    count_text_->SetActive(false);
    double_click_event_ = [](){};
    tooltip_ = nullptr;
}

void UIShopItemRow::Init()
{
    background_->SetSize(GetSize());
    count_text_->SetRelativePosition({GetSize().x - 80.f, 4.f});
    UIContainer::Init();
}

bool UIShopItemRow::OnMouseButton(const Math::Vector2& position, MouseButton button, bool is_pressed, double timestamp)
{
    bool result = UIContainer::OnMouseButton(position, button, is_pressed, timestamp);
    if (!is_pressed || button != MouseButton::kLeft || item_id_ == 0)
        return result;

    if (timestamp - last_click_time_ < .25f)
    {
        last_click_time_ = 0.0;
        if (double_click_event_)
            double_click_event_();
        return true;
    }

    last_click_time_ = timestamp;
    return true;
}

bool UIShopItemRow::OnMouseMotion(const Math::Vector2& position, const Math::Vector2& delta)
{
    if (!tooltip_)
        return false;

    EngineSettings* settings = EngineSettings::Get();
    Math::Vector2 tooltip_size = tooltip_->GetSize();
    Math::Vector2 tooltip_position = position + Math::Vector2::Up() * 32.f;

    int32_t screen_width = settings->GetScreenWidth();
    int32_t screen_height = settings->GetScreenHeight();

    int32_t overflow_width = static_cast<int32_t>(tooltip_position.x + tooltip_size.x - screen_width);
    int32_t overflow_height = static_cast<int32_t>(tooltip_position.y + tooltip_size.y - screen_height);

    if (overflow_width > 0) tooltip_position.x -= overflow_width;
    if (overflow_height > 0) tooltip_position.y -= overflow_height;

    tooltip_->SetAbsolutePosition(tooltip_position);
    return true;
}

bool UIShopItemRow::OnMouseEnter()
{
    if (item_id_ == 0)
        return false;

    auto* state = dynamic_cast<UIInGameState*>(UI::Get()->GetState());
    if (!state)
        return false;

    tooltip_ = state->GetItemTooltip();
    if (!tooltip_)
        return false;

    tooltip_->Set(item_id_);
    tooltip_->SetActive(true);
    return true;
}

bool UIShopItemRow::OnMouseLeave()
{
    if (!tooltip_)
        return false;

    tooltip_->SetActive(false);
    tooltip_ = nullptr;
    return true;
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIShopItemRow>("UIShopItemRow")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
