#include "pch.h"
#include "Map_Empty.h"

#include "Actors/PrototypeTilemap.h"

Map_Empty::Map_Empty(const std::wstring& kName) :
    Level(kName)
{
}

void Map_Empty::Load()
{
    Level::Load();
    
    AddActor<Actor>(PrototypeTilemap::StaticClass(), L"Tilemap");
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Map_Empty>("Map_Empty")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
