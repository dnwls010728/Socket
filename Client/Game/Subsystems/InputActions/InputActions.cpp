#include "pch.h"
#include "InputActions.h"

#include "GameInstance.h"
#include "Scancode.h"

InputActions::InputActions() :
    key_map_()
{
}

InputActions::Mapping InputActions::GetMapping(uint32_t scancode) const
{
    auto it = key_map_.find(scancode);
    if (it == key_map_.end())
        return { static_cast<uint8_t>(KeyType::kNone), 0 };
    return it->second;
}

InputActions::Mapping InputActions::GetMapping(Scancode scancode) const
{
    return GetMapping(static_cast<uint32_t>(scancode));
}

void InputActions::Bind(uint32_t scancode, uint8_t type, int32_t action)
{
    if (type == 0) return;
    key_map_[scancode] = { type, action };
}

void InputActions::Bind(Scancode scancode, KeyType type, int32_t action)
{
    Bind(static_cast<uint32_t>(scancode), static_cast<uint8_t>(type), action);
}

void InputActions::Unbind(uint32_t scancode)
{
    key_map_[scancode] = { static_cast<uint8_t>(KeyType::kNone), 0 };
}

void InputActions::Unbind(Scancode scancode)
{
    Unbind(static_cast<uint32_t>(scancode));
}

InputActions* InputActions::Get()
{
    return GameInstance::Get()->GetSubsystem<InputActions>();
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<InputActions>("InputActions")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
