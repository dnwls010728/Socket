#include "pch.h"
#include "PopupHandler.h"

#include <CustomPacket.h>

#include "Actors/NetworkActor.h"
#include "Subsystems/NetworkSubsystem.h"
#include "UI/Element/UIPopup.h"

bool PopupHandler::Handle(Net::IPacket* packet)
{
    PopupPacket* received_packet = dynamic_cast<PopupPacket*>(packet);
    if (!received_packet) return false;

    UIPopup::PopupParam param;
    param.caption = received_packet->text;
    param.option = UIPopup::PopupOption::OK;
    param.callback = [&](const std::wstring& text,  UIPopup::PopupOption option){ return true;};
    UIPopup::ShowPopup(param);
    return true;
}
