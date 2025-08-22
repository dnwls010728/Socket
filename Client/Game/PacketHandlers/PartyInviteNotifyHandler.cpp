#include "pch.h"
#include "PartyInviteNotifyHandler.h"

#include <CustomPacket.h>

#include "Actors/NetworkActor.h"
#include "Subsystems/NetworkSubsystem.h"
#include <sstream>

#include "Subsystems/SessionSubsystem.h"
#include "UI/Element/UIPopup.h"

bool PartyInviteNotifyHandler::Handle(Net::IPacket* packet)
{
    PartyInviteNotify* received_packet = dynamic_cast<PartyInviteNotify*>(packet);
    if (!received_packet) return false;
    
    std::wstringstream wss;
    wss << received_packet->inviter_name;
    wss << L"님이 \n";
    wss << L"[" << received_packet->party_name << L"]";
    wss << L"파티로 초대하였습니다.\n";
    wss << L"수락 하시겠습니까?";
            
    UIPopup::PopupParam param;
    param.caption = wss.str();
    param.option = UIPopup::PopupOption::Yes | UIPopup::PopupOption::No;
    param.callback = [party_id = received_packet->party_id, inviter_id = received_packet->inviter_id]
    (const std::wstring& input_text, UIPopup::PopupOption option)
    {
        PartyInviteNotifyResponse response;
        response.party_id = party_id;
        response.inviter_id = inviter_id;
        response.result = option == UIPopup::PopupOption::Yes;
        SessionSubsystem::Get()->SendPacket(response);
        return true;
    };
    UIPopup::ShowPopup(param);
    
    return true;
}
