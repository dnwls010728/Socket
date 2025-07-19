#pragma once
#include "Asset/Asset.h"
#include "Misc/StringHelper.h"

struct Animation
{
    std::wstring name;
    int32_t frame_rate;
    bool is_loop;
    std::vector<std::wstring> frame;
};

namespace YAML
{
    template<>
    struct convert<Animation>
    {
        static bool decode(const Node& node, Animation& animation)
        {
            if (!node.IsMap()) return false;

            animation.name = StringHelper::UTF8ToUTF16(node["name"].as<std::string>());
            animation.frame_rate = node["frame_rate"].as<int32_t>();
            animation.is_loop = node["loop"].as<bool>();

            for (const auto& frame : node["frames"])
            {
                animation.frame.push_back(StringHelper::UTF8ToUTF16(frame.as<std::string>()));
            }

            return true;
        }
    };
}

class AnimationPack : public Asset
{
    GENERATED_BODY(AnimationPack, Asset)
    
public:
    AnimationPack();
    virtual ~AnimationPack() override = default;

    virtual bool Load(const std::wstring& kPath) override;

private:
    friend class AnimatorComponent;
    
    std::wstring target_;
    
    std::map<std::wstring, Animation> animations_;
    
};
