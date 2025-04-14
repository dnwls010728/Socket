#pragma once
#include "GameInstanceSubsystem.h"

class TestSubsystem : public GameInstanceSubsystem
{
    GENERATED_BODY(TestSubsystem, GameInstanceSubsystem)
    
public:
    TestSubsystem();
    virtual ~TestSubsystem() override = default;

    virtual void Init() override;
    
};
