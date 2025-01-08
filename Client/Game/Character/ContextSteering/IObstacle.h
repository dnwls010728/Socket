#pragma once

class IObstacle
{
public:
    virtual class ColliderComponent* GetCollider() const = 0;
    
};
