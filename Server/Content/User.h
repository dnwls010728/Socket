#pragma once

#include "Award.h"
#include "GameSession.h"
class User
{
public:
    User() = default;
    ~User()
    {
        cout<< "User Memory Free \n";
    }
    
    User(const User&) = default;
    User& operator=(const User&) = default;
    User(User&&) = default;
    User& operator=(User&&) = default;

    string name;
    string id;
    uint32_t userIdentifyId;
    float locationX;
    float locationY;
    uint32_t characterId;
    uint32_t weaponId;
    weak_ptr<class Room> _ownerRoom;
    weak_ptr<class GameSession> ownerSession;
    vector<Award> awards;
};
