#pragma once
#include<rttr/registration>

#include "Misc/EngineMacros.h"
#include "rttr/registration_friend.h"
#include "yaml-cpp/yaml.h"

class Resource
{
    GENERATED_BODY(Resource);
    
public:
    Resource();
    virtual ~Resource() = default;

    virtual bool Load(const std::wstring& kPath);

protected:
    YAML::Node meta_data_;
    
};
