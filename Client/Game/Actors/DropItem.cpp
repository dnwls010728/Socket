#include "pch.h"
#include "DropItem.h"

DropItem::DropItem(const std::wstring& name) :
    NetworkActor(name)
{
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<DropItem>("DropItem")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
