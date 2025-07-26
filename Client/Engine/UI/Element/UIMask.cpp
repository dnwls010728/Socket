#include "pch.h"
#include "UIMask.h"

#include "Windows/DX/Renderer.h"

UIMask::UIMask(const std::wstring& name) :
    UIContainer(name)
{
}

void UIMask::Render()
{
    Renderer* renderer = Renderer::Get();

    renderer->BeginLayer(GetAbsolutePosition(), size_);
    UIContainer::Render();
    renderer->EndLayer();
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIMask>("UIMask")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
