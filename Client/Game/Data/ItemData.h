#pragma once

class UITexture;

class ItemData
{
public:
    enum class Category : uint8_t
    {
        kNone = 0,
        kEquip,
        kConsume,
        kEtc
    };
    
    ItemData(int32_t item_id);
    ~ItemData() = default;

    FORCEINLINE int32_t GetItemID() const { return item_id_; }
    FORCEINLINE int32_t GetPrice() const { return price_; }
    FORCEINLINE int32_t GetMaxStock() const { return max_stock_; }
    FORCEINLINE Category GetCategory() const { return category_; }
    FORCEINLINE const std::wstring& GetName() const { return name_; }
    FORCEINLINE const std::wstring& GetDesc() const { return desc_; }
    FORCEINLINE UITexture* GetIcon() const { return icon_; }
    FORCEINLINE bool IsValid() const { return is_valid_; }

private:
    int32_t item_id_;
    int32_t price_;
    int32_t max_stock_;

    Category category_;

    std::wstring name_;
    std::wstring desc_;
    
    UITexture* icon_;

    bool is_valid_;
    
};
