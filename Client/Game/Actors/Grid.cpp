#include "pch.h"
#include "Grid.h"

#include "Components/GridRendererComponent.h"

Grid::Grid(const std::wstring& name) :
    Actor(name)
{
    grid_renderer_ = AddComponent<GridRendererComponent>(L"GridRenderer");
}

void Grid::SetRows(uint32_t rows) const
{
    grid_renderer_->SetRows(rows);
}

void Grid::SetCols(uint32_t columns) const
{
    grid_renderer_->SetCols(columns);
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
