#include "pch.h"
#include "UIShop.h"

#include <algorithm>

#include <CustomPacket.h>

#include "Asset/AssetManager.h"
#include "Math/Color.h"
#include "Misc/Function.h"
#include "Subsystems/DataSubsystem.h"
#include "Subsystems/PlayerSubsystem.h"
#include "Subsystems/Publisher/PublisherSubsystem.h"
#include "Subsystems/SessionSubsystem.h"
#include "UI/Element/UIButton.h"
#include "UI/Element/UIImage.h"
#include "UI/Element/UIScrollBox.h"
#include "UI/Element/UIText.h"
#include "UI/Element/UIPopup.h"
#include "UI/Element/Shop/UIShopItemRow.h"
#include "Windows/DX/UISprite.h"

namespace
{
    const float kWindowWidth = 640.f;
    const float kWindowHeight = 560.f;
    const float kItemRowHeight = 48.f;
}

UIShop::UIShop(const std::wstring& name) :
    UIContainer(name),
    background_(nullptr),
    npc_panel_(nullptr),
    player_panel_(nullptr),
    npc_image_(nullptr),
    player_image_(nullptr),
    npc_title_text_(nullptr),
    player_money_text_(nullptr),
    close_button_(nullptr),
    player_tab_buttons_(),
    npc_scroll_box_(nullptr),
    player_scroll_box_(nullptr),
    npc_list_content_(nullptr),
    player_list_content_(nullptr),
    npc_entries_(),
    player_entries_(),
    player_tab_(InventoryType::kEquip),
    npc_id_(0)
{
    SetSize({ kWindowWidth, kWindowHeight });
    SetAbsolutePosition({ 250.f, 120.f });

    UISprite* panel_sprite   = AssetManager::Get()->Load<UISprite>(L"UI\\Panel.png");
    UISprite* button_sprite  = AssetManager::Get()->Load<UISprite>(L"UI\\Panel.png");
    UISprite* close_sprite   = AssetManager::Get()->Load<UISprite>(L"UI\\CloseButton.png");
    UISprite* character_sheet = AssetManager::Get()->Load<UISprite>(L"UI\\UIPlayerSheet.png");

    background_ = AddChild<UIImage>(UIImage::StaticClass(), L"Background");
    background_->SetSprite(panel_sprite, L"Panel_0");
    background_->SetDrawMode(UIImage::DrawMode::kSliced);
    background_->SetIgnoreRayCast(true);

    npc_panel_ = AddChild<UIImage>(UIImage::StaticClass(), L"NpcPanel");
    npc_panel_->SetSprite(panel_sprite, L"Panel_0");
    npc_panel_->SetDrawMode(UIImage::DrawMode::kSliced);
    npc_panel_->SetRelativePosition({ 12.f, 40.f });
    npc_panel_->SetSize({ 300.f, kWindowHeight - 60.f });
    npc_panel_->SetIgnoreRayCast(true);

    player_panel_ = AddChild<UIImage>(UIImage::StaticClass(), L"PlayerPanel");
    player_panel_->SetSprite(panel_sprite, L"Panel_0");
    player_panel_->SetDrawMode(UIImage::DrawMode::kSliced);
    player_panel_->SetRelativePosition({ kWindowWidth - 312.f, 40.f });
    player_panel_->SetSize({ 300.f, kWindowHeight - 60.f });
    player_panel_->SetIgnoreRayCast(true);

    npc_title_text_ = AddChild<UIText>(UIText::StaticClass(), L"NpcTitle");
    npc_title_text_->SetRelativePosition({ 20.f, 10.f });
    npc_title_text_->SetSize({ 280.f, 24.f });
    npc_title_text_->SetColor(Math::Color::White);
    npc_title_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    npc_title_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    npc_title_text_->SetText(L"NPC 상점");
    npc_title_text_->SetIgnoreRayCast(true);

    UIText* player_title = AddChild<UIText>(UIText::StaticClass(), L"PlayerTitle");
    player_title->SetRelativePosition({ kWindowWidth - 300.f, 10.f });
    player_title->SetSize({ 280.f, 24.f });
    player_title->SetColor(Math::Color::White);
    player_title->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    player_title->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    player_title->SetText(L"내 인벤토리");
    player_title->SetIgnoreRayCast(true);

    close_button_ = AddChild<UIButton>(UIButton::StaticClass(), L"CloseButton");
    close_button_->SetRelativePosition({ kWindowWidth - 34.f, 8.f });
    close_button_->SetSize({ 24.f, 24.f });
    close_button_->SetSprite(UIButton::State::kNormal, close_sprite, L"CloseButton_0");
    close_button_->SetSprite(UIButton::State::kHover, close_sprite, L"CloseButton_0");
    close_button_->SetSprite(UIButton::State::kPressed, close_sprite, L"CloseButton_0");
    close_button_->SetSprite(UIButton::State::kDisabled, close_sprite, L"CloseButton_0");
    close_button_->OnClick([this]()
    {
        ShopClosePacket packet;
        SessionSubsystem::Get()->SendPacket(packet);
        
        SetActive(false);
    });
    
    npc_image_ = AddChild<UIImage>(UIImage::StaticClass(), L"NpcImage");
    npc_image_->SetRelativePosition({ 28.f, 48.f });
    npc_image_->SetSize({ 160.f, 160.f });
    npc_image_->SetSprite(character_sheet, L"UIPlayerSheet_0");

    npc_scroll_box_ = AddChild<UIScrollBox>(UIScrollBox::StaticClass(), L"NpcScrollBox");
    npc_scroll_box_->SetRelativePosition({ 24.f, 48.f + 160.f + 12.f });
    npc_scroll_box_->SetSize({ 276.f, kWindowHeight - (48.f + 160.f + 32.f) });
    npc_scroll_box_->SetScrollStep(kItemRowHeight);
    npc_list_content_ = npc_scroll_box_->AddItem<UIContainer>(UIContainer::StaticClass(), L"NpcContent");
    npc_list_content_->SetSize({ npc_scroll_box_->GetSize().x - 12.f, npc_scroll_box_->GetSize().y });
    
    player_image_ = AddChild<UIImage>(UIImage::StaticClass(), L"PlayerImage");
    player_image_->SetRelativePosition({ kWindowWidth - 300.f + 16.f, 48.f });
    player_image_->SetSize({ 160.f, 160.f });
    // player_image_->SetSprite(character_sheet, L"UIPlayerSheet_0");

    UIButton* equip_button = AddChild<UIButton>(UIButton::StaticClass(), L"EquipTab");
    equip_button->SetRelativePosition({ kWindowWidth - 300.f, 48.f + 160.f + 12.f });
    equip_button->SetSize({ 70.f, 26.f });
    equip_button->SetSprite(UIButton::State::kNormal, button_sprite, L"Panel_0");
    equip_button->SetSprite(UIButton::State::kHover, button_sprite, L"Panel_0");
    equip_button->SetSprite(UIButton::State::kPressed, button_sprite, L"Panel_0");
    equip_button->SetSprite(UIButton::State::kDisabled, button_sprite, L"Panel_0");
    equip_button->SetText(L"장비");
    equip_button->SetTextColor(Math::Color::White);
    equip_button->SetDrawMode(UIImage::DrawMode::kSliced);
    equip_button->OnClick([this]() { SwitchPlayerTab(InventoryType::kEquip); });

    UIButton* use_button = AddChild<UIButton>(UIButton::StaticClass(), L"UseTab");
    use_button->SetRelativePosition({ kWindowWidth - 226.f, 48.f + 160.f + 12.f });
    use_button->SetSize({ 70.f, 26.f });
    use_button->SetSprite(UIButton::State::kNormal, button_sprite, L"Panel_0");
    use_button->SetSprite(UIButton::State::kHover, button_sprite, L"Panel_0");
    use_button->SetSprite(UIButton::State::kPressed, button_sprite, L"Panel_0");
    use_button->SetSprite(UIButton::State::kDisabled, button_sprite, L"Panel_0");
    use_button->SetText(L"소비");
    use_button->SetTextColor(Math::Color::White);
    use_button->SetDrawMode(UIImage::DrawMode::kSliced);
    use_button->OnClick([this]() { SwitchPlayerTab(InventoryType::kUse); });

    UIButton* etc_button = AddChild<UIButton>(UIButton::StaticClass(), L"EtcTab");
    etc_button->SetRelativePosition({ kWindowWidth - 152.f, 48.f + 160.f + 12.f });
    etc_button->SetSize({ 70.f, 26.f });
    etc_button->SetSprite(UIButton::State::kNormal, button_sprite, L"Panel_0");
    etc_button->SetSprite(UIButton::State::kHover, button_sprite, L"Panel_0");
    etc_button->SetSprite(UIButton::State::kPressed, button_sprite, L"Panel_0");
    etc_button->SetSprite(UIButton::State::kDisabled, button_sprite, L"Panel_0");
    etc_button->SetText(L"기타");
    etc_button->SetTextColor(Math::Color::White);
    etc_button->SetDrawMode(UIImage::DrawMode::kSliced);
    etc_button->OnClick([this]() { SwitchPlayerTab(InventoryType::kEtc); });

    player_tab_buttons_[static_cast<uint8_t>(InventoryType::kEquip)] = equip_button;
    player_tab_buttons_[static_cast<uint8_t>(InventoryType::kUse)]   = use_button;
    player_tab_buttons_[static_cast<uint8_t>(InventoryType::kEtc)]   = etc_button;

    player_scroll_box_ = AddChild<UIScrollBox>(UIScrollBox::StaticClass(), L"PlayerScrollBox");
    player_scroll_box_->SetRelativePosition({ kWindowWidth - 300.f, 48.f + 160.f + 12.f + 32.f });
    player_scroll_box_->SetSize({ 276.f, kWindowHeight - (48.f + 160.f + 64.f) });
    player_scroll_box_->SetScrollStep(kItemRowHeight);
    player_list_content_ = player_scroll_box_->AddItem<UIContainer>(UIContainer::StaticClass(), L"PlayerContent");
    player_list_content_->SetSize({ player_scroll_box_->GetSize().x - 12.f, player_scroll_box_->GetSize().y });
    
    player_money_text_ = AddChild<UIText>(UIText::StaticClass(), L"PlayerMoney");
    player_money_text_->SetRelativePosition({ kWindowWidth - 160.f, 225.f });
    player_money_text_->SetSize({ 150.f, 24.f });
    player_money_text_->SetColor(Math::Color::White);
    player_money_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
    player_money_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    player_money_text_->SetText(L"보유 금액: 0 컬러");
    player_money_text_->SetIgnoreRayCast(true);
}

void UIShop::OpenShop(int32_t npc_id, const std::vector<ShopItemInfo>& items)
{
    const NPCData* npc_data = DataSubsystem::Get()->GetNPC(npc_id);
    if (!npc_data) return;
    
    npc_id_ = npc_id;

    npc_title_text_->SetText(L"NPC 상점 - " + std::to_wstring(npc_id));

    // UISprite* character_sheet = AssetManager::Get()->Load<UISprite>(L"UI\\UIPlayerSheet.png");
    // uint32_t frame_index = npc_id % 7;
    // npc_image_->SetSprite(character_sheet, L"UIPlayerSheet_" + std::to_wstring(frame_index));

    UpdateNpcItems(items);
    RefreshPlayerInventory();
    UpdatePlayerMoney();

    SetActive(true);
}

void UIShop::Init()
{
    background_->SetSize(GetSize());
    npc_panel_->SetSize({ 300.f, kWindowHeight - 60.f });
    player_panel_->SetSize({ 300.f, kWindowHeight - 60.f });

    UIContainer::Init();

    PublisherSubsystem* publisher = PublisherSubsystem::Get();
    publisher->Subscribe(PublisherSubsystem::EventType::kItemAdded, this, &UIShop::OnEvent);
    publisher->Subscribe(PublisherSubsystem::EventType::kItemRemoved, this, &UIShop::OnEvent);
    publisher->Subscribe(PublisherSubsystem::EventType::kItemCountChanged, this, &UIShop::OnEvent);
    publisher->Subscribe(PublisherSubsystem::EventType::kItemMoved, this, &UIShop::OnEvent);
    publisher->Subscribe(PublisherSubsystem::EventType::kColorUpdated, this, &UIShop::OnEvent);

    SwitchPlayerTab(player_tab_);
    RefreshPlayerInventory();
    UpdatePlayerMoney();

    if (PlayerSubsystem* player = PlayerSubsystem::Get())
    {
        Math::Color color = Math::Color::HexToColor(player->GetBodyColor());
        player_image_->SetColor(color);
    }
}

void UIShop::Uninit()
{
    PublisherSubsystem* publisher = PublisherSubsystem::Get();
    publisher->Unsubscribe(PublisherSubsystem::EventType::kItemAdded, this, &UIShop::OnEvent);
    publisher->Unsubscribe(PublisherSubsystem::EventType::kItemRemoved, this, &UIShop::OnEvent);
    publisher->Unsubscribe(PublisherSubsystem::EventType::kItemCountChanged, this, &UIShop::OnEvent);
    publisher->Unsubscribe(PublisherSubsystem::EventType::kItemMoved, this, &UIShop::OnEvent);
    publisher->Unsubscribe(PublisherSubsystem::EventType::kColorUpdated, this, &UIShop::OnEvent);

    UIContainer::Uninit();
}

bool UIShop::OnDragBegin(const Math::Vector2&) { return true; }
bool UIShop::OnDrag(const Math::Vector2&, const Math::Vector2& delta)
{
    SetRelativePosition(GetRelativePosition() + delta);
    return true;
}
bool UIShop::OnDragEnd(const Math::Vector2&) { return true; }

void UIShop::UpdateNpcItems(const std::vector<ShopItemInfo>& items)
{
    EnsureNpcEntries(items.size());

    for (size_t i = 0; i < npc_entries_.size(); ++i)
    {
        UIShopItemRow* row = npc_entries_[i];
        if (i < items.size())
        {
            row->SetActive(true);
            row->SetItem(items[i].item_id);
            row->SetCountText(L"");
            row->SetPriceText(L"가격: " + FormatCurrency(items[i].price) + L" 컬러");

            row->SetDoubleClickHandler(Function<void(void)>(
                [this, item_id = items[i].item_id, price = items[i].price]()
                {
                    OnNpcItemDoubleClicked(item_id, price);
                }));
        }
        else
        {
            row->Clear();
            row->SetActive(false);
        }
    }
}

void UIShop::RefreshPlayerInventory()
{
    PlayerSubsystem* player = PlayerSubsystem::Get();
    if (!player) return;

    Inventory* inventory = player->GetInventory();
    if (!inventory) return;

    std::vector<std::pair<uint32_t, int32_t>> items;

    uint32_t capacity = inventory->GetSlotCapacity(player_tab_);
    for (uint32_t slot = 1; slot <= capacity; ++slot)
    {
        uint32_t item_id = inventory->GetItemID(player_tab_, slot);
        if (!item_id) continue;

        int32_t count = inventory->GetItemCount(player_tab_, slot);
        items.emplace_back(item_id, count);
    }

    AllocPlayerItemRow(items.size());

    for (size_t i = 0; i < player_entries_.size(); ++i)
    {
        UIShopItemRow* row = player_entries_[i];

        if (i < items.size())
        {
            row->SetActive(true);
            row->SetItem(items[i].first);
            // TODO :: 판매 가격 반영
            row->SetPriceText(L"판매 가격: " + FormatCurrency(1000) + L" 컬러");
            row->SetCountText(L"보유: " + std::to_wstring(items[i].second));

            row->SetDoubleClickHandler(Function<void(void)>(
                [this, item_id = items[i].first]() { OnPlayerItemDoubleClicked(item_id); }));
        }
        else
        {
            row->Clear();
            row->SetActive(false);
        }
    }
}

void UIShop::UpdatePlayerMoney(int32_t color)
{
    PlayerSubsystem* player = PlayerSubsystem::Get();
    if (!player) return;

    Inventory* inv = player->GetInventory();
    if (!inv) return;

    int32_t current = (color >= 0) ? color : inv->GetColor(); 
    player_money_text_->SetText(L"보유 금액: " + FormatCurrency(current) + L" 컬러");
}

void UIShop::OnNpcItemDoubleClicked(int32_t item_id, int32_t price)
{
    const ItemData* item = DataSubsystem::Get()->GetItem(item_id);
    std::wstring name = item ? item->name : L"알 수 없는 아이템";

    UIPopup::PopupParam param;
    param.caption = name + L"을(를) " + FormatCurrency(price) + L" 컬러에 구매하시겠습니까?";
    param.option = UIPopup::PopupOption::Yes | UIPopup::PopupOption::No;
    param.callback = [item_id, price, npc = npc_id_](const std::wstring&, UIPopup::PopupOption opt)
    {
        if (opt == UIPopup::PopupOption::Yes)
        {
            // TODO: 구매
        }
        return true;
    };

    UIPopup::ShowPopup(param);
}

void UIShop::OnPlayerItemDoubleClicked(int32_t item_id)
{
    const ItemData* item = DataSubsystem::Get()->GetItem(item_id);
    std::wstring name = item ? item->name : L"알 수 없는 아이템";

    // TODO :: 판매 가격 반영
    
    UIPopup::PopupParam param;
    param.caption = name + L"을(를) " + FormatCurrency(1000) + L" 컬러에 판매하시겠습니까?";
    param.option = UIPopup::PopupOption::Yes | UIPopup::PopupOption::No;
    param.callback = [](const std::wstring&, UIPopup::PopupOption) { return true; };

    UIPopup::ShowPopup(param);
}

std::wstring UIShop::FormatCurrency(int32_t value)
{
    std::wstring out = std::to_wstring(value);
    int32_t idx = out.size() - 3;
    while (idx > 0)
    {
        out.insert(idx, L",");
        idx -= 3;
    }
    return out;
}
void UIShop::EnsureNpcEntries(size_t count)
{
    while (npc_entries_.size() < count)
    {
        auto* itemRow = npc_list_content_->AddChild<UIShopItemRow>(
            UIShopItemRow::StaticClass(),
            L"NpcItemRow" + std::to_wstring(npc_entries_.size())
        );

        itemRow->SetSize({ npc_scroll_box_->GetSize().x - 12.f, kItemRowHeight - 6.f });
        npc_entries_.push_back(itemRow);
    }

    for (size_t i = 0; i < npc_entries_.size(); ++i)
        npc_entries_[i]->SetRelativePosition({ 0.f, static_cast<float>(i) * kItemRowHeight });

    float content_height = std::max(
        npc_scroll_box_->GetSize().y,
        static_cast<float>(count) * kItemRowHeight
    );

    npc_list_content_->SetSize({ npc_scroll_box_->GetSize().x - 12.f, content_height });
}

void UIShop::AllocPlayerItemRow(size_t count)
{
    while (player_entries_.size() < count)
    {
        auto* itemRow = player_list_content_->AddChild<UIShopItemRow>(
            UIShopItemRow::StaticClass(),
            L"PlayerItemRow" + std::to_wstring(player_entries_.size())
        );

        itemRow->SetSize({ player_scroll_box_->GetSize().x - 12.f, kItemRowHeight - 6.f });
        player_entries_.push_back(itemRow);
    }

    for (size_t i = 0; i < player_entries_.size(); ++i)
        player_entries_[i]->SetRelativePosition({ 0.f, static_cast<float>(i) * kItemRowHeight });

    float content_height = std::max(
        player_scroll_box_->GetSize().y,
        static_cast<float>(count) * kItemRowHeight
    );

    player_list_content_->SetSize({ player_scroll_box_->GetSize().x - 12.f, content_height });
}

void UIShop::SwitchPlayerTab(InventoryType type)
{
    if (player_tab_buttons_[static_cast<uint8_t>(player_tab_)])
        player_tab_buttons_[static_cast<uint8_t>(player_tab_)]->SetTextColor(Math::Color::White);

    player_tab_ = type;

    if (player_tab_buttons_[static_cast<uint8_t>(player_tab_)])
        player_tab_buttons_[static_cast<uint8_t>(player_tab_)]->SetTextColor(Math::Color::Yellow);

    RefreshPlayerInventory();
}

// TODO: 함수 분리
void UIShop::OnEvent(const EventData& data)
{
    if (dynamic_cast<const ItemAddedData*>(&data) ||
        dynamic_cast<const ItemRemovedData*>(&data) ||
        dynamic_cast<const ItemCountChangedData*>(&data) ||
        dynamic_cast<const ItemMovedData*>(&data))
    {
        RefreshPlayerInventory();
    }
    else if (const auto* color_update = dynamic_cast<const ColorUpdateData*>(&data))
    {
        UpdatePlayerMoney(color_update->color);
    }
}


RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIShop>("UIShop")
        .constructor<const std::wstring&>()
        (policy::ctor::as_raw_ptr);
}
