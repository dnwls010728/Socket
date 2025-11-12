#pragma once
#include <memory>

class ShopItem : public std::enable_shared_from_this<ShopItem>
{
public:
    ShopItem(int32_t item_id, int32_t price);
    ~ShopItem() = default;
    
};
