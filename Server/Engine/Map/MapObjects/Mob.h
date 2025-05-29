#pragma once
#include "Engine/Map/MapObject.h"

class Mob : public MapObject
{
public:
    Mob();
    virtual ~Mob() override = default;

protected:
    virtual void Tick(float delta_time) override;

    Math::Vector2 velocity_;

    float gravity_;
    
};
