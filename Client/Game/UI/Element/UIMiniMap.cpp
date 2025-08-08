#include "pch.h"
#include "UIMiniMap.h"

#include "Actor/Component/Tilemap/Tilemap.h"
#include "Asset/AssetManager.h"
#include "UI/Element/UIImage.h"
#include "UI/Element/UIText.h"
#include "Windows/DX/UISprite.h"

UIMiniMap::UIMiniMap(const std::wstring& name) :
    UIContainer(name)
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

void UIMiniMap::SetTilemap(const Tilemap* tilemap)
{
    if (!tilemap) return;
    
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
}

void UIMiniMap::Init()
{
    frame_->SetSize(GetSize());
    
    UIContainer::Init();
}

void UIMiniMap::Render()
{
    UIContainer::Render();
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
