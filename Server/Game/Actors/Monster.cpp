#include "Monster.h"

Monster::Monster(std::wstring kName) : Actor(kName)
{
}

void Monster::Tick(float delta_time)
{
    Actor::Tick(delta_time);
    
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