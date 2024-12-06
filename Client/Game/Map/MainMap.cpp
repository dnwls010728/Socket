#include "pch.h"
#include "MainMap.h"

#include "Actor/Tilemap.h"
#include "Character/Actor/Mob/1/Mob1.h"

MainMap::MainMap(const std::wstring& kName) : Level(kName)
{
}

void MainMap::Load()
{
    Level::Load();

    Tilemap* tilemap = AddActor<Tilemap>(L"Tilemap");
    Mob1* mob1 = AddActor<Mob1>(L"Mob1");
}

RTTR_REGISTRATION
{
    rttr::registration::class_<MainMap>("MainMap")
        .constructor<const std::wstring&>()
        (
            rttr::policy::ctor::as_std_shared_ptr
        );
}
