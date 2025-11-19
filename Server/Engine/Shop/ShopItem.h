#pragma once
#include <memory>

class ShopItem : public std::enable_shared_from_this<ShopItem>
{
public:
    ShopItem(int32_t item_id, int32_t price);
    ~ShopItem() = default;
    
    int32_t GetItemID() const { return item_id_; }
    int32_t GetPrice() const { return price_; }
    
private:
    int32_t item_id_;
    int32_t price_;
    
};
