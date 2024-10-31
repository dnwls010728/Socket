#pragma once
#include "Actor/Actor.h"

class Image;

class Inventory : public Actor
{
    SHADER_CLASS_HELPER(Inventory)
    GENERATED_BODY(Inventory, Actor);
    
public:
    Inventory(const std::wstring& kName);
    virtual ~Inventory() override = default;

private:
    Image* window_caption_;
    Image* window_panel_;
    
};
