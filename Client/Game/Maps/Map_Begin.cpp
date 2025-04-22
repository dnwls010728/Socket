#include "pch.h"
#include "Map_Begin.h"

#include "Actors/PrototypeTilemap.h"

Map_Begin::Map_Begin(const std::wstring& kName) :
    Level(kName)
{
}

void Map_Begin::Load()
{
    Level::Load();

    AddActor<Actor>(PrototypeTilemap::StaticClass(), L"Tilemap");
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Map_Begin>("Map_Begin")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
