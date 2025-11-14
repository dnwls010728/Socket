#include "pch.h"
#include "ShopOpenHandler.h"

#include <CustomPacket.h>

bool ShopOpenHandler::Handle(Net::IPacket* packet)
{
    ShopOpenResponse* response = dynamic_cast<ShopOpenResponse*>(packet);
    if (!response) return false;
    
    return true;
}
