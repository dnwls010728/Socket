#include "pch.h"
#include "TilemapComponent.h"

#include "TilemapLayer.h"
#include "Actor/Actor.h"
#include "Actor/Component/TransformComponent.h"
#include "box2d/box2d.h"
#include "Level/World.h"
#include "Asset/AssetManager.h"
#include "Subsystems/NetworkSubsystem.h"
#include "Windows/DX/Sprite.h"

TilemapComponent::TilemapComponent(Actor* owner, const std::wstring& kName) :
	ActorComponent(owner, kName),
	kPPU(32.f),
	map_size_(Math::Vector2::Zero()),
	tilemap_layers_(),
	collision_bodies_()
{
}

void TilemapComponent::LoadMap(const char* kPath)
{
	map_.load(kPath);

	map_size_.x = static_cast<float>(map_.getTileCount().x);
	map_size_.y = static_cast<float>(map_.getTileCount().y);
	
	const auto& layers = map_.getLayers();
	for (const auto& layer : layers)
	{
		if (layer->getType() == tmx::Layer::Type::Object)
		{
			const auto& object = layer->getLayerAs<tmx::ObjectGroup>();
			
			if (layer->getName() == "Collision") GeneratePhysics(object);
			else if (layer->getName() == "Spawn") GenerateSpawn(object);
		}
		else if (layer->getType() == tmx::Layer::Type::Tile)
		{
			const auto& tile_layer = layer->getLayerAs<tmx::TileLayer>();
			
			Math::Vector2 chunk_size = {512.f, 512.f};
			tilemap_layers_.emplace_back(std::make_unique<TilemapLayer>(map_, tile_layer, chunk_size));
		}
	}
}

void TilemapComponent::InitializeComponent()
{
	ActorComponent::InitializeComponent();

	if (b2Body_IsValid(tilemap_body_id_)) b2Body_Enable(tilemap_body_id_);
}

void TilemapComponent::UninitializeComponent()
{
	ActorComponent::UninitializeComponent();

	if (b2Body_IsValid(tilemap_body_id_)) b2DestroyBody(tilemap_body_id_);
}

void TilemapComponent::BeginPlay()
{
	ActorComponent::BeginPlay();

	GET_NETWORK()->SetTilemapComponent(GetSharedThis());
}

void TilemapComponent::Render(float alpha)
{
	ActorComponent::Render(alpha);

	for (const auto& tilemap_layer : tilemap_layers_)
	{
		tilemap_layer->UpdateShapes(
			GetOwner()->GetTransform()->GetPosition(),
			{ 1.f / kPPU, 1.f / kPPU },
			{ map_size_.x / 2.f, -(map_size_.y / 2.f) }
		);
	}
}

void TilemapComponent::GeneratePhysics(const tmx::ObjectGroup& kObject)
{
	const auto& objects = kObject.getObjects();

	b2BodyDef body_def = b2DefaultBodyDef();
	body_def.userData = GetOwner();
	
	tilemap_body_id_ = b2CreateBody(World::Get()->world_id_, &body_def);

	for (const auto& temp : objects)
	{
		b2Polygon shape;
		
		if (temp.getShape() == tmx::Object::Shape::Rectangle)
		{
			b2Vec2 center = {temp.getPosition().x / kPPU + ((temp.getAABB().width / 2) / kPPU) - map_size_.x / 2.f, -1 * temp.getPosition().y / kPPU - ((temp.getAABB().height / 2) / kPPU) + map_size_.y / 2.f};
			shape = b2MakeOffsetBox(temp.getAABB().width / 2 / kPPU, temp.getAABB().height / 2 / kPPU, center, b2Rot_identity);

			collision_bodies_.push_back({{center.x, center.y}, {temp.getAABB().width / kPPU, temp.getAABB().height / kPPU}});
		}
		else if (temp.getShape() == tmx::Object::Shape::Polygon)
		{
			std::vector<b2Vec2> vertices;
			
			for (const auto& point : temp.getPoints())
			{
				b2Vec2 vertex = {point.x / kPPU + temp.getPosition().x / kPPU - map_size_.x / 2.f, -1 * point.y / kPPU - temp.getPosition().y / kPPU + map_size_.y / 2.f};
				vertices.push_back(vertex);
			}

			b2Hull hull = b2ComputeHull(vertices.data(), vertices.size());
			shape = b2MakePolygon(&hull, 0.f);
		}
		
		b2Filter filter = b2DefaultFilter();
		filter.categoryBits = static_cast<Type::uint16>(GetOwner()->GetLayer());
		filter.maskBits = static_cast<Type::uint16>(EngineSettings::Get()->GetCollisionLayer(GetOwner()->GetLayer()));
		
		b2ShapeDef shape_def = b2DefaultShapeDef();
		shape_def.filter = filter;

		b2CreatePolygonShape(tilemap_body_id_, &shape_def, &shape);
	}

	b2Body_Disable(tilemap_body_id_);
}

void TilemapComponent::GenerateSpawn(const tmx::ObjectGroup& kObject)
{
	const auto& objects = kObject.getObjects();

	for (const auto& temp : objects)
	{
		if (temp.getShape() == tmx::Object::Shape::Point)
		{
			rttr::type type = rttr::type::get_by_name(temp.getClass());
			if (type.is_valid())
			{
				const std::string& name = temp.getName();
				
				std::wstring to_wide_string = std::wstring(name.begin(), name.end());
				std::shared_ptr<Actor> actor = World::Get()->SpawnActor<Actor>(type, to_wide_string);
				if (IsValid(actor))
				{
					std::shared_ptr<TransformComponent> transform = actor->GetTransform();
					transform->SetPosition({temp.getPosition().x / kPPU - map_size_.x / 2.f, -1 * temp.getPosition().y / kPPU + map_size_.y / 2.f});
				}
			}
		}
	}
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
