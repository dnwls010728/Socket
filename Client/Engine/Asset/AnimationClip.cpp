#include "pch.h"
#include "AnimationClip.h"

#include "Data/FileHelper.h"

AnimationClip::AnimationClip() :
    data_(YAML::Null)
{
}

bool AnimationClip::Load(const std::wstring& kPath)
{
    Asset::Load(kPath);
    if (!FileHelper::IsFileExists(kPath)) return false;

    try
    {
        const std::string kToString = std::string(kPath.begin(), kPath.end());
        data_ = YAML::LoadFile(kToString);
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

    registration::class_<AnimationClip>("AnimationClip")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
