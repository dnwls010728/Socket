#include "pch.h"
#include "PlacementSubsystem.h"

#include <CustomPacket.h>

#include "SessionSubsystem.h"
#include "Actor/Component/TransformComponent.h"
#include "Actor/Component/Tilemap/TilemapComponent.h"
#include "Actors/BlockPreview.h"
#include "Actors/Grid.h"
#include "DirectXTK/Mouse.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Windows/DX/Renderer.h"

PlacementSubsystem::PlacementSubsystem() :
    grid_(nullptr),
    is_placing_(false),
    tilemap_component_(nullptr)
{
}

void PlacementSubsystem::OnWorldBeginPlay()
{
    WorldSubsystem::OnWorldBeginPlay();
    if (!tilemap_component_) return;

    grid_ = World::Get()->SpawnActor<Grid>(Grid::StaticClass(), L"Grid");
    if (IsValid(grid_))
    {
        const auto& map_size = tilemap_component_->GetMapSize();
        grid_->SetRows(map_size.y);
        grid_->SetCols(map_size.x);
        grid_->SetActive(false);
    }

    block_preview_ = World::Get()->SpawnActor<BlockPreview>(BlockPreview::StaticClass(), L"BlockPreview");
    if (IsValid(block_preview_))
    {
        block_preview_->SetActive(false);
    }
    
}

void PlacementSubsystem::Tick(float delta_time)
{
    Tickable::Tick(delta_time);

    Keyboard* keyboard = Keyboard::Get();
    if (keyboard->GetKeyDown(Scancode::kKeyEscape) && is_placing_)
    {
        PlacementStopRequest request;
        SessionSubsystem::Get()->SendPacket(request);
    }

    Mouse* mouse = Mouse::Get();

    Math::Vector2 mouse_position = mouse->GetMousePosition();
    Math::Vector2 world_position = Renderer::Get()->ScreenToWorld(mouse_position);

    if (is_placing_)
    {
        Math::Vector2i cell_position = tilemap_component_->WorldToCell(world_position);
        Math::Vector2 cell_center = tilemap_component_->GetCellCenter(cell_position);
        
        if (IsValid(block_preview_))
        {
            block_preview_->GetTransform()->SetPosition(cell_center);
        }

        if (mouse->GetMouseButtonDown(MouseButton::kLeft))
        {
            PlacementBlockPacket packet;
            packet.position.x = cell_center.x;
            packet.position.y = cell_center.y;
            SessionSubsystem::Get()->SendPacket(packet);
        }
    }
}

void PlacementSubsystem::StartPlacement()
{
    if (IsValid(grid_)) grid_->SetActive(true);
    if (IsValid(block_preview_)) block_preview_->SetActive(true);
    is_placing_ = true;
}

void PlacementSubsystem::StopPlacement()
{
    if (IsValid(grid_)) grid_->SetActive(false);
    if (IsValid(block_preview_)) block_preview_->SetActive(false);
    is_placing_ = false;
}

PlacementSubsystem* PlacementSubsystem::Get()
{
    return World::Get()->GetSubsystem<PlacementSubsystem>();
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<PlacementSubsystem>("PlacementSubsystem")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
