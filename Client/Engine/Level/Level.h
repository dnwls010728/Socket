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
    GENERATED_BODY(Level);
    
public:
    Level(const std::wstring& kName);
    virtual ~Level() = default;

    virtual void Load() = 0;
    
    virtual void Unload(EndPlayReason type);
    virtual void InitializeActors();
    virtual void PhysicsTick(float delta_time);
    virtual void Tick(float delta_time);
    virtual void PostTick(float delta_time);
    virtual void Render(float alpha);

    inline const std::wstring& GetName() const { return name_; }
    inline bool HasBegunPlay() const { return has_begun_play_; }

protected:
    template<std::derived_from<Actor> T>
    T* AddActor(const std::wstring& kName);

private:
    friend class World;
    friend class Editor;

    void OnEvent(const Event& event);
    
    std::wstring name_;

    std::vector<std::shared_ptr<Actor>> actors_;

    bool has_begun_play_;
};

template <std::derived_from<Actor> T>
T* Level::AddActor(const std::wstring& kName)
{
    std::shared_ptr<Actor> actor = std::make_shared<T>(kName);
    actors_.push_back(actor);

    return static_cast<T*>(actor.get());
}
