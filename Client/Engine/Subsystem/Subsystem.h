#pragma once

class Subsystem
{
    GENERATED_BODY(Subsystem)
    
public:
    Subsystem();
    virtual ~Subsystem() = default;

    virtual void Init();
    virtual void Deinit();
    
};
