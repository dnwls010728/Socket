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
        uint8_t type;
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
    Mapping GetMapping(Scancode scancode) const;
    
    void Bind(uint32_t scancode, uint8_t type, int32_t action);
    void Bind(Scancode scancode, KeyType type, int32_t action);
    void Unbind(uint32_t scancode);
    void Unbind(Scancode scancode);

    static InputActions* Get();

private:
    std::map<uint32_t, Mapping> key_map_;
    
};
