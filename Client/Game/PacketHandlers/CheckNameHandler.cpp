#include "pch.h"
#include "CheckNameHandler.h"

#include <CustomPacket.h>

#include "UI/UIPopup.h"
#include "UI/UIState.h"
#include "UI/Element/UICharacterCreate.h"

bool CheckNameHandler::Handle(Net::IPacket* packet)
{
    CheckNameResponse* received_packet = dynamic_cast<CheckNameResponse*>(packet);
    if (!received_packet) return false;

    if (received_packet->is_available)
    {
        UIPopup::ShowPopup(L"사용 가능한 이름입니다.", PopupOption::OK, [&](std::wstring input_text, PopupOption option)
        {
            return true;
        });
    }
    else
    {
        UIPopup::ShowPopup(L"이미 사용 중인 이름입니다.", PopupOption::OK, [&](std::wstring input_text, PopupOption option)
        {
            return true;
        });
    }

    if (auto* state = UI::Get()->GetState())
    {
        if (UICharacterCreate* element = state->FindElement<UICharacterCreate>(L"CharacterCreate"))
            element->OnCheckResult(received_packet->is_available);
    }

    return true;
}
