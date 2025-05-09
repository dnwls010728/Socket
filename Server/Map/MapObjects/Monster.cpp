#include "Monster.h"
#include "rttr/registration"

Monster::Monster(std::wstring kName) : MapObject(kName)
{
}

void Monster::Tick(float delta_time)
{
    MapObject::Tick(delta_time);
    
}


RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Monster>("Monster")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}