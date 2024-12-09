#pragma once
#include<rttr/registration>

#include "Misc/EngineMacros.h"
#include "rttr/registration_friend.h"
#include "yaml-cpp/yaml.h"

class Asset
{
    GENERATED_BODY(Asset);
    
public:
    Asset();
    virtual ~Asset() = default;

    virtual bool Load(const std::wstring& kPath);

protected:
    YAML::Node meta_data_;
    
};
