#pragma once

#include "GameSession.h"
class User
{
public:
    User() = default;
    ~User()
    {
        cout<< "User Memory Free" << endl;
    }

    string name;
    string id;
    uint32_t userIdentifyId;
    int32_t locationX;
    int32_t locationY;
    uint16_t characterId;
    weak_ptr<class GameSession> ownerSession;
};
