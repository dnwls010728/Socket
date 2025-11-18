#pragma once
#include "CommonObject.h"
#include "Inventory/Inventory.h"
#include "Subsystems/Publisher/PublisherSubsystem.h"
#include "UI/UIContainer.h"

#include <array>
#include <vector>

class UIButton;
class UIImage;
class UIText;
class UIScrollBox;
class UIShopItemRow;
class UIItemTooltip;
struct ShopBuyResponse;
struct ShopSellPriceResponse;
struct ShopSellResponse;

class UIShop : public UIContainer
{
    GENERATED_BODY(UIShop, UIContainer)

public:
    UIShop(const std::wstring& name);
    virtual ~UIShop() override = default;

    void OpenShop(int32_t npc_id, const std::vector<ShopItemInfo>& items);
    void HandleSellPriceResponse(const ShopSellPriceResponse& response);
    void HandleSellResponse(const ShopSellResponse& response);
    void HandleBuyResponse(const ShopBuyResponse& response);

protected:
    virtual void Init() override;
    virtual void Uninit() override;

    virtual bool OnDragBegin(const Math::Vector2& position) override;
    virtual bool OnDrag(const Math::Vector2& position, const Math::Vector2& delta) override;
    virtual bool OnDragEnd(const Math::Vector2& position) override;

private:
    void AllocNPCItemRow(size_t count);
    void AllocPlayerItemRow(size_t count);
    void UpdateNpcItems(const std::vector<ShopItemInfo>& items);
    void RefreshPlayerInventory();
    void UpdatePlayerMoney(int32_t color = -1);
    void SwitchPlayerTab(InventoryType type);
    void OnNpcItemDoubleClicked(int32_t item_id, int32_t price);
    void OnPlayerItemDoubleClicked(InventoryType type, uint32_t slot_id, int32_t row_dix);
    void RequestSellPrice(InventoryType type, uint32_t slot_id, int32_t count);
    void ShowSellPopup(InventoryType type, uint32_t slot_id, uint32_t item_id, int32_t price, int32_t count);
    void OnEvent(const EventData& data);

    static std::wstring FormatCurrency(int32_t value);

    struct PlayerInventoryEntry
    {
        uint32_t slot_id;
        uint32_t item_id;
        int32_t count;
    };

    UIImage* background_;
    UIImage* npc_panel_;
    UIImage* player_panel_;
    UIImage* npc_image_;
    UIImage* player_image_;

    UIText* npc_title_text_;
    UIText* player_money_text_;

    UIButton* close_button_;
    std::array<UIButton*, static_cast<size_t>(InventoryType::kCount)> player_tab_buttons_;

    UIScrollBox* npc_scroll_box_;
    UIScrollBox* player_scroll_box_;

    UIContainer* npc_list_content_;
    UIContainer* player_list_content_;

    std::vector<UIShopItemRow*> npc_item_rows_;
    std::vector<UIShopItemRow*> player_item_rows_;
    std::vector<PlayerInventoryEntry> player_items_;

    UIItemTooltip* tooltip_;

    InventoryType player_tab_;

    int32_t npc_id_;

};
