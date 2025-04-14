#pragma once
#include "Subsystems/Tickable.h"
#include "Subsystems/WorldSubsystem.h"

class TestWorldSubsystem : public WorldSubsystem, public Tickable
{
    GENERATED_BODY(TestWorldSubsystem, WorldSubsystem)
    
public:
    TestWorldSubsystem();
    virtual ~TestWorldSubsystem() override = default;

    virtual void Init() override;
    virtual void Deinit() override;
    virtual void Tick(float delta_time) override;
    
};
