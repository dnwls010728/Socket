#include "pch.h"
#include "InputActions.h"

#include "GameInstance.h"
#include "Scancode.h"

InputActions::InputActions() :
    key_map_()
{
    key_map_[static_cast<uint32_t>(Scancode::kKey0)] = { KeyType::kItem, 200000 };
    key_map_[static_cast<uint32_t>(Scancode::kKeyQ)] = { KeyType::kSkill, 100000 };
}

InputActions::Mapping InputActions::GetMapping(uint32_t scancode) const
{
    auto it = key_map_.find(scancode);
    if (it == key_map_.end())
        return { KeyType::kNone, 0 };
    return it->second;
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
