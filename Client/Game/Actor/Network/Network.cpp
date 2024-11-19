#include "pch.h"
#include "Network.h"

Network::Network(const std::wstring& kName) :
    Actor(kName)
{
}

void Network::Tick(float delta_time)
{
    Actor::Tick(delta_time);
    
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Network>("Network")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
