#pragma once
#include "GameInstanceSubsystem.h"

class TestSubsystem : public GameInstanceSubsystem
{
    SHADER_CLASS_HELPER(TestSubsystem)
    GENERATED_BODY(TestSubsystem, GameInstanceSubsystem)
    
public:
    TestSubsystem();
    virtual ~TestSubsystem() override = default;
    
};
