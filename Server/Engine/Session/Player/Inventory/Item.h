#pragma once
#include <memory>

class Item : public std::enable_shared_from_this<Item>
{
public:
    Item(uint32_t id, int32_t count);
    virtual ~Item() = default;

    inline uint32_t GetID() const { return id_; }

    inline void SetSlot(uint32_t slot) { slot_ = slot; }
    inline uint32_t GetSlot() const { return slot_; }

    inline void SetCount(int32_t count) { count_ = count; }
    inline int32_t GetCount() const { return count_; }

    static std::shared_ptr<Item> Create(uint32_t id, int32_t count);

private:
    uint32_t id_;
    uint32_t slot_;

    int32_t count_;
    
};
