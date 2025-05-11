#pragma once
#include "UI/Widget.h"

class InventoryData;

namespace UI
{
    class Inventory : public Widget
    {
        SHADER_CLASS_HELPER(Inventory)
        GENERATED_BODY(Inventory, Widget)
        
    public:
        Inventory(const std::wstring& name);
        virtual ~Inventory() override = default;

        static std::shared_ptr<Inventory> Create(const std::wstring& name);

    protected:
        virtual void Render(Renderer* renderer, WindowsWindow* window) override;
        virtual void OnAdd() override;

        virtual bool OnDragBegin(const Math::Vector2& position) override;
        virtual bool OnDrag(const Math::Vector2& position, const Math::Vector2& delta) override;
        virtual bool OnDragEnd(const Math::Vector2& position) override;

        uint16_t GetSlotByPosition(const Math::Vector2& position) const;

    private:
        int16_t slot_row_;
        int16_t slot_col_;
        int16_t max_slots_;

        InventoryData* inventory_data_;
        
        uint16_t dragged_slot_;
        
        bool is_dragging_;
    
    };
}
