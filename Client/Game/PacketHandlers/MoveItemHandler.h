#pragma once
#include "IPacketHandler.h"

// TODO: 패킷 수정 필요
class MoveItemHandler : public IPacketHandler
{
public:
    virtual ~MoveItemHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;
    
};
