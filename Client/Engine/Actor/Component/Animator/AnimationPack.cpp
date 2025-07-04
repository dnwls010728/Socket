#include "pch.h"
#include "AnimationPack.h"

#include "Data/FileHelper.h"
#include "Misc/StringHelper.h"

AnimationPack::AnimationPack() :
    target_(L""),
    animations_()
{
}

bool AnimationPack::Load(const std::wstring& kPath)
{
    Asset::Load(kPath);
    if (!FileHelper::IsFileExists(kPath)) return false;

    try
    {
        std::string to_string = std::string(kPath.begin(), kPath.end());
        YAML::Node node = YAML::LoadFile(to_string);

        if (!node.IsNull())
        {
            target_ = StringHelper::UTF8ToUTF16(node["target"].as<std::string>());

            if (node["animations"].IsSequence())
            {
                for (const YAML::Node& animation : node["animations"])
                {
                    Animation temp = animation.as<Animation>();
                    animations_[temp.name] = temp;
                }
            }
        }
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

    registration::class_<AnimationPack>("AnimationPack")
        .constructor<>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
