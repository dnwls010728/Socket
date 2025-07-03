#pragma once
#include "Singleton.h"

class DataManager : public Singleton<DataManager>
{
public:
    DataManager();
    virtual ~DataManager() override = default;

    void Init();
    
};
