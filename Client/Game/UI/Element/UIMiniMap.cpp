#include "pch.h"
#include "UIMiniMap.h"

#include "Asset/AssetManager.h"
#include "UI/Element/UIImage.h"
#include "UI/Element/UIText.h"
#include "Windows/DX/UISprite.h"

UIMiniMap::UIMiniMap(const std::wstring& name) :
    UIContainer(name),
    markers_()
{
    SetSize({ 200.f, 200.f });
    
    UISprite* panel_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\Panel.png");
    
    frame_ = AddChild<UIImage>(UIImage::StaticClass(), L"Frame");
    frame_->SetSprite(panel_sprite, L"Panel_0");
    frame_->SetDrawMode(UIImage::DrawMode::kSliced);
    
    map_ = AddChild<UIImage>(UIImage::StaticClass(), L"Map");

    map_name_text_ = AddChild<UIText>(UIText::StaticClass(), L"MapNameText");
    map_name_text_->SetAbsolutePosition({ 4.f, 4.f });
    map_name_text_->SetSize({ 192.f, 20.f });
    map_name_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    map_name_text_->SetColor(Math::Color::White);
    map_name_text_->SetText(L"알 수 없는 곳");
}

void UIMiniMap::Init()
{
    UIContainer::Init();

    frame_->SetSize(GetSize());
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
