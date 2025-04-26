#pragma once
#include "../MapObject.h"

class Monster : public MapObject
{
public:
    Monster();
    virtual ~Monster() override = default;
    
};
