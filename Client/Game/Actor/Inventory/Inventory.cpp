#include "pch.h"
#include "Inventory.h"

#include "UI/Canvas.h"

Inventory::Inventory(const std::wstring& kName) :
    Actor(kName),
    slots_(),
    slot_count_(35),
    row_count_(5)
{
    Canvas* canvas = Canvas::Get();

    for (int i = 0; i < slot_count_; ++i)
    {
        int row = i / row_count_;
        int col = i % row_count_;

        std::unique_ptr<Slot> slot = std::make_unique<Slot>(canvas->GetRootWidget(), Math::Vector2(col * 40.0f, row * 40.0f), i);
        slots_.push_back(std::move(slot));
    }
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Inventory>("Inventory")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
