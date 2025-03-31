#include "pch.h"
#include "TempMap.h"

#include "Actors/Characters/Player/PlayerCharacter.h"
#include "Actors/Tilemaps/DefaultTilemap.h"
#include "Editor/Editor.h"
#include "UI/UIManager.h"
#include "Widgets/ServerList.h"

TempMap::TempMap(const std::wstring& kName) :
    Level(kName)
{
}

void TempMap::Load()
{
    Level::Load();
    
    // AddActor<PlayerCharacter>(L"Player");
    // AddActor<DefaultTilemap>(L"Tilemap");
    // AddActor<Editor>(L"Editor");

    std::shared_ptr<UI::ServerList> server_list = std::make_shared<UI::ServerList>(L"ServerList");
    server_list->SetPosition({320.f, 240.f});
    server_list->SetSize({300.f, 150.f});
    
    UI::Manager::Get()->AddToViewport(server_list);
    
}

void TempMap::Tick(float delta_time)
{
    Level::Tick(delta_time);

}

RTTR_REGISTRATION
{
    using namespace rttr;
    
    registration::class_<TempMap>("TempMap")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
