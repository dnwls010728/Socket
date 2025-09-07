#pragma once
#include "Item.h"

class EquipItem : public Item
{
public:
    EquipItem(uint32_t id);
    virtual ~EquipItem() override = default;
    
    EquipItem(const EquipItem&) = default;

    inline int32_t GetReqLv() const { return req_lv_; }
    inline int32_t GetMaxHP() const { return max_hp_; }
    inline int32_t GetAtk() const { return atk_; }
    inline int32_t GetDef() const { return def_; }
    inline int32_t GetDig() const { return dig_; }

    virtual std::shared_ptr<Item> Clone() const override;
    
    static std::shared_ptr<EquipItem> Create(uint32_t id);

private:
    int32_t req_lv_;
    int32_t max_hp_;
    int32_t atk_;
    int32_t def_;
    int32_t dig_;
    
};
