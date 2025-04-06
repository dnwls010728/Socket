#pragma once
#include <queue>
#include <vector>
#include <rttr/registration>

#include "Enums.h"
#include "Level.h"
#include "PhysicsDebugDrawHelper.h"
#include "Singleton.h"
#include "Actor/Actor.h"
#include "box2d/id.h"
#include "box2d/types.h"

union Event;
class ShapeBatch;
class Level;
class Shape;
enum class LevelType : Type::uint64;
class WindowsWindow;

struct ActorActivation
{
    Actor* actor;
    bool is_active;
};

class World : public Singleton<World>
{
public:
    World();
    virtual ~World() override;

    void Init(const std::shared_ptr<WindowsWindow>& kWindow);
    void OpenLevel(const std::wstring& kName);
    void PhysicsTick(float delta_time);
    void Tick(float delta_time);
    void PostTick(float delta_time);
    void Render(float alpha);
    void AddShape(const std::shared_ptr<Shape>& kShape);
    void RemoveShape(const std::shared_ptr<Shape>& kShape);

    void GetActors(const rttr::type& type, std::vector<Actor*>& actors);

    Actor* GetActor(const rttr::type& type);

    template<std::derived_from<Actor> T>
    T* SpawnActor(const rttr::type& kType, const std::wstring& kName = L"");

    template<std::derived_from<Level> T>
    void AddLevel(const std::wstring& kName);

    FORCEINLINE WindowsWindow* GetWindow() const { return window_.get(); }
    FORCEINLINE Level* GetLevel() const { return current_level_; }

private:
    friend void DrawPolygon(const b2Vec2* vertices, int vertexCount, b2HexColor color, void* context);
    friend void DrawSolidPolygon(b2Transform transform, const b2Vec2* vertices, int vertexCount, float radius, b2HexColor color, void* context);
    friend void DrawCircle(b2Vec2 center, float radius, b2HexColor color, void* context);
    friend void DrawSolidCircle(b2Transform transform, float radius, b2HexColor color, void* context);
    friend void DrawSolidCapsule(b2Vec2 p1, b2Vec2 p2, float radius, b2HexColor color, void* context);
    friend void DrawSegment(b2Vec2 p1, b2Vec2 p2, b2HexColor color, void* context);
    friend void DrawTransform(b2Transform transform, void* context);
    friend void DrawPoint(b2Vec2 p, float size, b2HexColor color, void* context);
    friend void DrawString(b2Vec2 p, const char* s, b2HexColor color, void* context);

    friend class Core;
    friend class GameEngine;
    friend class Editor;
    friend class Physics2D;
    friend class Level;
    friend class Actor;
    friend class SpriteRendererComponent;
    friend class TilemapComponent;
    
    void TransitionLevel();
    void ProcessCollisionEvents();
    void ProcessTriggerEvents();
    void ProcessActorActivation();
    void SpawnActors();
    void DestroyActor(Actor* actor);
    void DestroyActors();
    void ActivateActor(Actor* actor, bool is_active);
    void SortZOrder();

    std::shared_ptr<WindowsWindow> window_;
    
    std::shared_ptr<ShapeBatch> shape_batch_;
    
    std::vector<std::shared_ptr<Shape>> shapes_;

    b2WorldId world_id_;
    b2DebugDraw debug_draw_;
    
    PhysicsDebugDrawHelper debug_draw_helper_;
    
    Level* current_level_;
    Level* persistent_level_;
    Level* pending_level_;

    std::unordered_map<std::wstring, std::shared_ptr<Level>> levels_;

    std::queue<std::shared_ptr<Actor>> pending_actors_;
    std::queue<std::shared_ptr<Actor>> pending_destroy_actors_;
    std::queue<ActorActivation> pending_actor_activation_;
};

template <std::derived_from<Actor> T>
T* World::SpawnActor(const rttr::type& kType, const std::wstring& kName)
{
    std::wstring name = kName;
    
    if (name.empty())
    {
        std::string type_name = kType.get_name().to_string();
        name.assign(type_name.begin(), type_name.end());
    }
    
    rttr::variant var = kType.create({ name });
    if (var.is_valid())
    {
        std::shared_ptr<Actor> actor = var.get_value<std::shared_ptr<Actor>>();
        pending_actors_.push(actor);

        actor->InitializeActor();

        rttr::type actor_type = rttr::type::get<T>();
        if (kType.is_derived_from(actor_type))
        {
            return static_cast<T*>(actor.get());
        }
    }

    return nullptr;
}

template <std::derived_from<Level> T>
void World::AddLevel(const std::wstring& kName)
{
    std::shared_ptr<Level> level = std::make_shared<T>(kName);
    levels_[kName] = level;
}
