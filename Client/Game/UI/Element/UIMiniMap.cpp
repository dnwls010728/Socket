#include "pch.h"
#include "UIMiniMap.h"

#include "Actor/Component/TransformComponent.h"
#include "Actor/Component/Tilemap/Tilemap.h"
#include "Actors/Characters/Player/PlayerCharacter.h"
#include "Asset/AssetManager.h"
#include "Subsystems/NetworkSubsystem.h"
#include "UI/Element/UIImage.h"
#include "UI/Element/UIText.h"
#include "Windows/DX/UISprite.h"

UIMiniMap::UIMiniMap(const std::wstring& name) :
    UIContainer(name),
    tilemap_(nullptr),
    marker_scale_(Math::Vector2::Zero())
{
    SetSize({ 200.f, 200.f });
    
    UISprite* panel_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\Panel.png");
    
    frame_ = AddChild<UIImage>(UIImage::StaticClass(), L"Frame");
    frame_->SetSprite(panel_sprite, L"Panel_0");
    frame_->SetDrawMode(UIImage::DrawMode::kSliced);
    
    map_ = AddChild<UIImage>(UIImage::StaticClass(), L"Map");
    map_->SetRelativePosition({4.f, 28.f});

    map_name_text_ = AddChild<UIText>(UIText::StaticClass(), L"MapNameText");
    map_name_text_->SetRelativePosition({ 4.f, 4.f });
    map_name_text_->SetSize({ 192.f, 20.f });
    map_name_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    map_name_text_->SetColor(Math::Color::White);
    map_name_text_->SetText(L"알 수 없는 곳");
}

void UIMiniMap::SetTilemap(Tilemap* tilemap)
{
    if (!tilemap) return;
    tilemap_ = tilemap;
    
    map_->SetSprite(tilemap->GetUISprite());
    
    Math::Vector2 map_size = tilemap->GetMapSize();
    Math::Vector2 tile_size = tilemap->GetTileSize();
    
    float width = (map_size.x * tile_size.x) * .25f;
    float height = (map_size.y * tile_size.y) * .25f;

    map_->SetSize({ width, height });

    SetSize({ width + 8.f, height + 32.f });
    frame_->SetSize(GetSize());

    map_name_text_->SetText(tilemap->GetName());
    map_name_text_->SetSize({map_name_text_->GetTotalAdvance() + 1.f, 20.f});

    Math::Vector2 size = map_->GetSize();

    Bounds world_bounds = tilemap->GetWorldBounds();
    float dx = world_bounds.max.x - world_bounds.min.x;
    float dy = world_bounds.max.y - world_bounds.min.y;

    marker_scale_.x = size.x / dx;
    marker_scale_.y = size.y / dy;
}

void UIMiniMap::Init()
{
    frame_->SetSize(GetSize());
    
    UIContainer::Init();
}

void UIMiniMap::Render()
{
    Renderer* renderer = Renderer::Get();
    UIContainer::Render();
    if (!tilemap_) return;

    NetworkSubsystem* subsystem = NetworkSubsystem::Get();
    Bounds world_bounds = tilemap_->GetWorldBounds();

    auto player = subsystem->GetPlayer();
    if (IsValid(player))
    {
        Math::Vector2 position = player->GetTransform()->GetPosition();
        
        Math::Vector2 marker_position;
        marker_position.x = (position.x - world_bounds.min.x) * marker_scale_.x + 4.f;
        marker_position.y = (world_bounds.max.y - position.y) * marker_scale_.y + 28.f;

        renderer->DrawSolidCircle(marker_position, 5.f, Math::Color::Yellow);
    }

    std::vector<std::shared_ptr<PlayerCharacter>> players = {};
    subsystem->GetOtherPlayers(players);

    for (const auto& other_player : players)
    {
        if (!IsValid(other_player)) continue;

        Math::Vector2 position = other_player->GetTransform()->GetPosition();
        
        Math::Vector2 marker_position;
        marker_position.x = (position.x - world_bounds.min.x) * marker_scale_.x + 4.f;
        marker_position.y = (world_bounds.max.y - position.y) * marker_scale_.y + 28.f;

        renderer->DrawSolidCircle(marker_position, 5.f, Math::Color::Red);
    }
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIMiniMap>("UIMiniMap")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
