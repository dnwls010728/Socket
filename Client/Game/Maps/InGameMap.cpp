#include "pch.h"
#include "InGameMap.h"

#include "Actors/TilemapLoader.h"

InGameMap::InGameMap(const std::wstring& kName) :
    Level(kName)
{
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<InGameMap>("InGameMap")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
