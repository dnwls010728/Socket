#include "pch.h"
#include "AnimationPack.h"

#include "Animation.h"
#include "Data/FileHelper.h"

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
            std::string target = node["target"].as<std::string>();
            target_ = std::wstring(target.begin(), target.end());

            if (node["animations"].IsSequence())
            {
                for (const YAML::Node& animation : node["animations"])
                {
                    std::string name = animation["name"].as<std::string>();
                    std::wstring to_wide_string(name.begin(), name.end());
                    
                    std::shared_ptr<Animation> data = std::make_shared<Animation>(to_wide_string);
                    data->frame_rate_ = animation["frame_rate"].as<int>();
                    data->is_loop_ = animation["loop"].as<bool>();

                    for (const YAML::Node& index : animation["frames"])
                    {
                        std::string frame = index.as<std::string>();
                        data->frames_.push_back(std::wstring(frame.begin(), frame.end()));
                    }

                    animations_[to_wide_string] = data;
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
