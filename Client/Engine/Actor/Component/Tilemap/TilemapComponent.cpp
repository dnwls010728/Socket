#include "pch.h"
#include "TilemapComponent.h"

#include "Tilemap.h"
#include "TilemapLayer.h"
#include "Actor/Actor.h"
#include "Actor/Portal.h"
#include "Actor/Component/TransformComponent.h"
#include "box2d/box2d.h"
#include "Level/World.h"
#include "Asset/AssetManager.h"
#include "Misc/StringHelper.h"
#include "Windows/DX/Sprite.h"

TilemapComponent::TilemapComponent(Actor* owner, const std::wstring& kName) :
	ActorComponent(owner, kName),
	ppu_(0.f),
	tilemap_(nullptr),
	map_size_(Math::Vector2::Zero()),
	tile_size_(Math::Vector2::Zero()),
	tilemap_layers_(),
	type_map_()
{
	tilemap_body_id_ = b2_nullBodyId;
	bounds_body_id_ = b2_nullBodyId;
}

void TilemapComponent::SetTilemap(Tilemap* tilemap)
{
	tilemap_ = tilemap;
}

int32_t TilemapComponent::GetType(const b2ShapeId shape_id)
{
	uint64_t id = b2StoreShapeId(shape_id);
	
	auto it = type_map_.find(id);
	if (it != type_map_.end()) return it->second;
	return -1;
}

Math::Vector2i TilemapComponent::WorldToCell(const Math::Vector2& position) const
{
	const auto& bounds = tilemap_->GetWorldBounds();

	float dx = position.x - bounds.min.x;
	float dy = bounds.max.y - position.y;

	int32_t tile_x = static_cast<int32_t>(std::floor(dx));
	int32_t tile_y = static_cast<int32_t>(std::floor(dy));

	tile_x = std::clamp(tile_x, 0, static_cast<int32_t>(map_size_.x) - 1);
	tile_y = std::clamp(tile_y, 0, static_cast<int32_t>(map_size_.y) - 1);
	return {tile_x, tile_y};
}

Math::Vector2 TilemapComponent::GetCellCenter(const Math::Vector2i& position) const
{
	const auto& bounds = tilemap_->GetWorldBounds();
	float x = bounds.min.x + (position.x + .5f);
	float y = bounds.max.y - (position.y + .5f);
	return {x, y};
}

void TilemapComponent::UninitializeComponent()
{
	ActorComponent::UninitializeComponent();

	for (const auto& tilemap_layer : tilemap_layers_)
	{
		tilemap_layer->RemoveShapes();
	}
}

void TilemapComponent::BeginPlay()
{
	ActorComponent::BeginPlay();
	
	if (tilemap_)
	{
		const tmx::Map& map = tilemap_->GetMap();

		ppu_ = tilemap_->GetPPU();
		
		map_size_.x = static_cast<float>(map.getTileCount().x);
		map_size_.y = static_cast<float>(map.getTileCount().y);

		tile_size_.x = static_cast<float>(map.getTileSize().x);
		tile_size_.y = static_cast<float>(map.getTileSize().y);
	
		const auto& layers = map.getLayers();
		for (const auto& layer : layers)
		{
			if (layer->getType() == tmx::Layer::Type::Object)
			{
				const auto& object = layer->getLayerAs<tmx::ObjectGroup>();
			
				if (layer->getName() == "Foothold") GeneratePhysics(object);
				else if (layer->getName() == "Spawn") GenerateSpawn(object);
				else if (layer->getName() == "Portal") GeneratePortal(object);
			}
			else if (layer->getType() == tmx::Layer::Type::Tile)
			{
				const auto& tile_layer = layer->getLayerAs<tmx::TileLayer>();
			
				Math::Vector2 chunk_size = {512.f, 512.f};
				tilemap_layers_.emplace_back(std::make_unique<TilemapLayer>(map, tile_layer, chunk_size));
			}
		}

		GenerateBounds();
	}
	
	if (b2Body_IsValid(tilemap_body_id_)) b2Body_Enable(tilemap_body_id_);
	if (b2Body_IsValid(bounds_body_id_)) b2Body_Enable(bounds_body_id_);

}

void TilemapComponent::EndPlay(EndPlayReason type)
{
	ActorComponent::EndPlay(type);
	
	if (b2Body_IsValid(tilemap_body_id_)) b2DestroyBody(tilemap_body_id_);
	if (b2Body_IsValid(bounds_body_id_)) b2DestroyBody(bounds_body_id_);
}

void TilemapComponent::Render(float alpha)
{
	ActorComponent::Render(alpha);

	for (const auto& tilemap_layer : tilemap_layers_)
	{
		tilemap_layer->UpdateShapes(
			GetOwner()->GetTransform()->GetPosition(),
			{ 1.f / ppu_, 1.f / ppu_ },
			{ map_size_.x / 2.f, -(map_size_.y / 2.f) }
		);
	}
}

void TilemapComponent::OnEnable()
{
	ActorComponent::OnEnable();

	for (const auto& tilemap_layer : tilemap_layers_)
	{
		tilemap_layer->AddShapes();
	}
}

void TilemapComponent::OnDisable()
{
	ActorComponent::OnDisable();

	for (const auto& tilemap_layer : tilemap_layers_)
	{
		tilemap_layer->RemoveShapes();
	}
}

void TilemapComponent::GeneratePhysics(const tmx::ObjectGroup& kObject)
{
	const auto& objects = kObject.getObjects();

	b2BodyDef body_def = b2DefaultBodyDef();
	body_def.userData = GetOwner();
	
	tilemap_body_id_ = b2CreateBody(World::Get()->world_id_, &body_def);

	float point = 0.f;
	
	for (const auto& temp : objects)
	{
		const auto& properties = temp.getProperties();
		
		b2Filter filter = b2DefaultFilter();
		filter.categoryBits = static_cast<uint16_t>(GetOwner()->GetLayer());
		filter.maskBits = static_cast<uint16_t>(EngineSettings::Get()->GetCollisionLayer(GetOwner()->GetLayer()));
		
		b2ShapeDef shape_def = b2DefaultShapeDef();
		shape_def.filter = filter;
		shape_def.userData = nullptr;
		
		b2ShapeId shape_id = b2_nullShapeId;
		
		if (temp.getShape() == tmx::Object::Shape::Rectangle)
		{
			b2Vec2 center = {temp.getPosition().x / ppu_ + ((temp.getAABB().width / 2) / ppu_) - map_size_.x / 2.f, -1 * temp.getPosition().y / ppu_ - ((temp.getAABB().height / 2) / ppu_) + map_size_.y / 2.f};
			b2Polygon shape = b2MakeOffsetBox(temp.getAABB().width / 2 / ppu_, temp.getAABB().height / 2 / ppu_, center, b2Rot_identity);
			
			shape_id = b2CreatePolygonShape(tilemap_body_id_, &shape_def, &shape);
			
			if (properties.size() > 0)
			{
				uint64_t id = b2StoreShapeId(shape_id);
				type_map_[id] = properties[0].getIntValue();
			}
		}
		else if (temp.getShape() == tmx::Object::Shape::Polyline)
		{
			const auto& points = temp.getPoints();
			for (int32_t i = 0; i < points.size() - 1; ++i)
			{
				b2Segment segment;
				segment.point1 = {
					points[i].x / ppu_ + temp.getPosition().x / ppu_ - map_size_.x / 2.f,
					-1 * points[i].y / ppu_ - temp.getPosition().y / ppu_ + map_size_.y / 2.f
				};

				segment.point2 = {
					points[i + 1].x / ppu_ + temp.getPosition().x / ppu_ - map_size_.x / 2.f,
					-1 * points[i + 1].y / ppu_ - temp.getPosition().y / ppu_ + map_size_.y / 2.f
				};

				shape_id = b2CreateSegmentShape(tilemap_body_id_, &shape_def, &segment);
				
				if (properties.size() > 0)
				{
					uint64_t id = b2StoreShapeId(shape_id);
					type_map_[id] = properties[3].getIntValue();
				}
			}
		}
	}

	b2Body_Disable(tilemap_body_id_);
}

void TilemapComponent::GenerateSpawn(const tmx::ObjectGroup& kObject) const
{
	const auto& objects = kObject.getObjects();

	for (const auto& temp : objects)
	{
		if (temp.getShape() != tmx::Object::Shape::Point) continue;
		
		rttr::type type = rttr::type::get_by_name(temp.getClass());
		if (type.is_valid())
		{
			std::wstring name = StringHelper::UTF8ToUTF16(temp.getName());
			std::shared_ptr<Actor> actor = World::Get()->SpawnActor<Actor>(type, name);
			if (IsValid(actor))
			{
				actor->GetTransform()->SetPosition({temp.getPosition().x / ppu_ - map_size_.x / 2.f, -1 * temp.getPosition().y / ppu_ + map_size_.y / 2.f});
			}
		}
	}
}

void TilemapComponent::GeneratePortal(const tmx::ObjectGroup& kObject) const
{
	const auto& objects = kObject.getObjects();

	for (const auto& temp : objects)
	{
		if (temp.getShape() != tmx::Object::Shape::Point) continue;

		const auto& properties = temp.getProperties();
		if (properties.empty()) continue;

		int32_t id = properties[0].getIntValue();
		
		rttr::type type = rttr::type::get_by_name(temp.getClass());
		if (type.is_valid())
		{
			std::wstring name = StringHelper::UTF8ToUTF16(temp.getName());
			std::shared_ptr<Portal> portal = World::Get()->SpawnActor<Portal>(type, name);
			if (IsValid(portal))
			{
				portal->GetTransform()->SetPosition({temp.getPosition().x / ppu_ - map_size_.x / 2.f, -1 * temp.getPosition().y / ppu_ + map_size_.y / 2.f});
				portal->SetID(id);
			}
		}
	}
}

void TilemapComponent::GenerateBounds()
{
	b2BodyDef body_def = b2DefaultBodyDef();
	body_def.userData = GetOwner();

	bounds_body_id_ = b2CreateBody(World::Get()->world_id_, &body_def);

	float width = (map_size_.x * tile_size_.x) / ppu_;
	float height = (map_size_.y * tile_size_.y) / ppu_;
	float half_width = width * .5f;
	float half_height = height * .5f;

	Math::Vector2 position = GetOwner()->GetTransform()->GetPosition();
	
	b2Filter filter = b2DefaultFilter();
	filter.categoryBits = static_cast<uint16_t>(GetOwner()->GetLayer());
	filter.maskBits = static_cast<uint16_t>(EngineSettings::Get()->GetCollisionLayer(GetOwner()->GetLayer()));
		
	b2ShapeDef shape_def = b2DefaultShapeDef();
	shape_def.filter = filter;
	shape_def.userData = nullptr;

	b2Segment segment;
	segment.point1 = { position.x - half_width, position.y - half_height };
	segment.point2 = { position.x + half_width, position.y - half_height };
	b2CreateSegmentShape(bounds_body_id_, &shape_def, &segment);

	segment.point1 = { position.x - half_width, position.y + half_height };
	segment.point2 = { position.x + half_width, position.y + half_height };
	b2CreateSegmentShape(bounds_body_id_, &shape_def, &segment);

	segment.point1 = { position.x - half_width, position.y - half_height };
	segment.point2 = { position.x - half_width, position.y + half_height };
	b2CreateSegmentShape(bounds_body_id_, &shape_def, &segment);

	segment.point1 = { position.x + half_width, position.y - half_height };
	segment.point2 = { position.x + half_width, position.y + half_height };
	b2CreateSegmentShape(bounds_body_id_, &shape_def, &segment);
	
	b2Body_Disable(tilemap_body_id_);
}

RTTR_REGISTRATION
{
	using namespace rttr;

	registration::class_<TilemapComponent>("TilemapComponent")
		.constructor<Actor*, const std::wstring&>()
		(
			policy::ctor::as_std_shared_ptr
		);
}
