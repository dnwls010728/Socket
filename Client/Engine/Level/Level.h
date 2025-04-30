#pragma once
#include <memory>
#include <string>
#include <vector>
#include <rttr/registration>

#include "Misc/EngineMacros.h"
#include "rttr/registration_friend.h"

union Event;
class Actor;
enum class EndPlayReason : Type::uint64;

class Level : public std::enable_shared_from_this<Level>
{
    SHADER_CLASS_HELPER(Level)
    GENERATED_BODY(Level)
    
public:
    Level(const std::wstring& kName);
    virtual ~Level() = default;

    FORCEINLINE const std::wstring& GetName() const { return name_; }
    FORCEINLINE bool HasBegunPlay() const { return has_begun_play_; }

protected:
    friend class GameEngine;
    
    virtual void Load();
    virtual void Unload(EndPlayReason type);
    virtual void InitializeActors();
    virtual void PhysicsTick(float delta_time);
    virtual void Tick(float delta_time);
    virtual void PostTick(float delta_time);
    virtual void Render(float alpha);
    
    template<std::derived_from<Actor> T>
    T* AddActor(const rttr::type& kType, const std::wstring& kName);

    FORCEINLINE const std::vector<std::shared_ptr<Actor>>& GetActors() const { return actors_; }

private:
    friend class World;
    friend class Editor;
    
    std::wstring name_;

    std::vector<std::shared_ptr<Actor>> actors_;

    bool has_begun_play_;
};

template <std::derived_from<Actor> T>
T* Level::AddActor(const rttr::type& kType, const std::wstring& kName)
{
    rttr::variant var = kType.create({ kName });
    if (var.is_valid())
    {
        std::shared_ptr<Actor> actor = var.get_value<std::shared_ptr<Actor>>();
        actors_.push_back(actor);

        rttr::type actor_type = rttr::type::get<T>();
        if (kType.is_derived_from(actor_type))
        {
            return static_cast<T*>(actor.get());
        }

        return nullptr;
    }
    
    return nullptr;
}
