#pragma once

#include "GameSession.h"
class User
{
public:
    User() = default;
    ~User()
    {
        cout<< "User Memory Free \n";
    }

    string name;
    string id;
    uint32_t userIdentifyId;
    float locationX;
    float locationY;
    uint16_t characterId;
    weak_ptr<class Room> _ownerRoom;
    weak_ptr<class GameSession> ownerSession;
};
