#pragma once
#include "Map/MapObject.h"

class Block : public MapObject
{
public:
    Block(const std::wstring& color, int32_t hp);
    virtual ~Block() override = default;

    virtual void SendSpawn(const std::shared_ptr<PlayerCharacter>& player) override;

private:
    std::wstring color_;
    
    std::atomic_int32_t hp_;
    
};
