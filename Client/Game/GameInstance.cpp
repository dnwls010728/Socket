#include "pch.h"
#include "GameInstance.h"

#include "Data/CSVReader.h"

GameInstance::GameInstance()
{
}

void GameInstance::Init()
{
    CSVReader::Parse(L"Data\\PropData.csv", prop_data_);
    
}

void GameInstance::Shutdown()
{
}

const PropData* GameInstance::GetPropData(int id)
{
    auto it = std::ranges::find_if(prop_data_, [id](const PropData& data) {
        return data.id == id;
    });

    if (it != prop_data_.end()) return &(*it);
    return nullptr;
}
