#include "pch.h"
#include "AnimationSet.h"

#include "Data/FileHelper.h"

AnimationSet::AnimationSet() :
    data_(YAML::Null),
    target_(L""),
    sequences_()
{
}

bool AnimationSet::Load(const std::wstring& kPath)
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

    if (data_.IsNull()) return false;

    std::string target = data_["target"].as<std::string>();
    target_ = std::wstring(target.begin(), target.end());

    if (data_["sequences"].IsSequence())
    {
        sequences_.clear();
        for (const YAML::Node& sequence : data_["sequences"])
        {
            std::string sequence_name = sequence["name"].as<std::string>();
            
            AnimationSequence data;
            data.name = std::wstring(sequence_name.begin(), sequence_name.end());
            data.sample_frame_rate = sequence["sample_frame_rate"].as<int>();
            data.is_loop = sequence["loop"].as<bool>();

            for (const YAML::Node& frame : sequence["frames"])
            {
                std::string frame_name = frame.as<std::string>();
                data.frames.push_back(std::wstring(frame_name.begin(), frame_name.end()));
            }

            sequences_[std::wstring(sequence_name.begin(), sequence_name.end())] = data;
        }
    }

    return true;
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<AnimationSet>("AnimationSet")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
