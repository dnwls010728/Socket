#include "pch.h"
#include "Asset.h"

#include "Data/FileHelper.h"

Asset::Asset() :
    meta_data_(YAML::Null)
{
}

bool Asset::Load(const std::wstring& kPath)
{
    if (!FileHelper::IsFileExists(kPath + L".meta")) return false;

    try
    {
        const std::string kToString = std::string(kPath.begin(), kPath.end());
        meta_data_ = YAML::LoadFile(kToString + ".meta");
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
    
    registration::class_<Asset>("Asset")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
