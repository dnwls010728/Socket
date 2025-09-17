#include "pch.h"
#include "DoSelectCardHandler.h"

#include <CustomPacket.h>
#include "UI/UIState.h"

#include "UI/Element/Card/UICardSelectPanel.h"

bool DoSelectCardHandler::Handle(Net::IPacket* packet)
{
    DoSelectCardPacket* received_packet = dynamic_cast<DoSelectCardPacket*>(packet);
    if (!received_packet) return false;

    UIState* state = UI::Get()->GetState();
    if (!state) return true;

    UICardSelectPanel *panel =  state->FindElementShallow<UICardSelectPanel>(L"CardSelectPanel");
    if (panel == nullptr || panel->IsPendingRemoval())
    {
        panel = state->AddElement<UICardSelectPanel>(UICardSelectPanel::StaticClass(), L"CardSelectPanel");
        Math::Vector2 pos{
            std::round((EngineSettings::Get()->GetScreenWidth() - panel->GetSize().x) / 2),
            std::round((EngineSettings::Get()->GetScreenHeight() - panel->GetSize().y) / 2)
        };
    
        panel->SetAbsolutePosition(pos);
    }
    
    panel->AddCards(received_packet->cards);
    panel->SetActive(true);

    return true;
}
