#include "pch.h"
#include "TempMap.h"

TempMap::TempMap(const std::wstring& kName) :
    Level(kName)
{
}

void TempMap::Load()
{
    Level::Load();
    
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
