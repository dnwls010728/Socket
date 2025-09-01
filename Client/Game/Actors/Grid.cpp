#include "pch.h"
#include "Grid.h"

#include "Components/GridRendererComponent.h"

Grid::Grid(const std::wstring& name) :
    Actor(name)
{
    grid_renderer_ = AddComponent<GridRendererComponent>(L"GridRenderer");
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Grid>("Grid")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
