#pragma once
#include "KeyType.h"
#include "Scancode.h"
#include "Subsystem/GameInstanceSubsystem.h"

class InputActions : public GameInstanceSubsystem
{
    GENERATED_BODY(InputActions, GameInstanceSubsystem)
    
public:
    struct Mapping
    {
        KeyType type;
        int32_t action;

        bool operator==(const Mapping& other) const
        {
            return type == other.type && action == other.action;
        }

        bool operator!=(const Mapping& other) const
        {
            return type != other.type || action != other.action;
        }
    };

    InputActions();
    virtual ~InputActions() override = default;

    Mapping GetMapping(uint32_t scancode) const;

    static InputActions* Get();

private:
    std::map<uint32_t, Mapping> key_map_;
    
};
