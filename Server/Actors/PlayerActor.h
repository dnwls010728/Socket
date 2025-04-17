#pragma once
#include "Actor.h"

class PlayerActor : public Actor
{
public:
    PlayerActor();
    virtual ~PlayerActor() override = default;

    inline virtual Type GetActorType() const override { return Type::kPlayer; }

    inline uint32_t GetCharacterID() const { return character_id_; }
    inline void SetCharacterID(uint32_t character_id) { character_id_ = character_id; }

private:
    uint32_t character_id_;
    
};
