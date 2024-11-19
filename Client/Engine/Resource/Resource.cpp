#include "pch.h"
#include "Resource.h"

#include "Data/FileHelper.h"

Resource::Resource() :
    meta_data_(YAML::Null)
{
}

bool Resource::Load(const std::wstring& kPath)
{
    if (!FileHelper::IsFileExists(kPath + L".yaml")) return false;

    try
    {
        const std::string kPathToString = std::string(kPath.begin(), kPath.end());
        meta_data_ = YAML::LoadFile(kPathToString + ".yaml");
    }
    catch (const YAML::BadFile& e)
    {
        return false;
    }
    
    return true;
}

RTTR_REGISTRATION
{
    using namespace rttr;
    
    registration::class_<Resource>("Resource")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
