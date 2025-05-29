#pragma once
#include "Engine/Map/MapObject.h"

class Mob : public MapObject
{
public:
    Mob();
    virtual ~Mob() override = default;

protected:
    friend class Map;
    
    virtual void Tick(float delta_time) override;

    Math::Vector2 velocity_;
    Math::Vector2 last_position_;

    float gravity_;
    
};
