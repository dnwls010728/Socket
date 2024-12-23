#include "pch.h"
#include "MainMap.h"

#include "Actor/Tilemap.h"
#include "Character/Mob/Dummy/Dummy.h"
#include "Character/Player/PlayerCharacter.h"

MainMap::MainMap(const std::wstring& kName) : Level(kName)
{
}

void MainMap::Load()
{
    Level::Load();

    Tilemap* tilemap = AddActor<Tilemap>(L"Tilemap");
    PlayerCharacter* player = AddActor<PlayerCharacter>(L"PlayerCharacter");
    Dummy* dummy = AddActor<Dummy>(L"Dummy");
}

RTTR_REGISTRATION
{
    rttr::registration::class_<MainMap>("MainMap")
        .constructor<const std::wstring&>()
        (
            rttr::policy::ctor::as_std_shared_ptr
        );
}
