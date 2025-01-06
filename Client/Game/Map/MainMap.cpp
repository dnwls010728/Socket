#include "pch.h"
#include "MainMap.h"

#include "Actor/Tilemap.h"
#include "Prop/Block.h"

MainMap::MainMap(const std::wstring& kName) : Level(kName)
{
}

void MainMap::Load()
{
    Level::Load();

    Tilemap* tilemap = AddActor<Tilemap>(L"Tilemap");
}

RTTR_REGISTRATION
{
    rttr::registration::class_<MainMap>("MainMap")
        .constructor<const std::wstring&>()
        (
            rttr::policy::ctor::as_std_shared_ptr
        );
}
