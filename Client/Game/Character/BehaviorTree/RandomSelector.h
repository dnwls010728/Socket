#pragma once
#include "Selector.h"

namespace BT
{
    class RandomSelector : public Selector
    {
    public:
        RandomSelector(const std::wstring& kName);
        virtual ~RandomSelector() override = default;

        virtual Status Process() override;

        virtual void Reset() override;

    private:
        const std::vector<std::shared_ptr<Node>>& GetShuffledChildren();
        
        const std::vector<std::shared_ptr<Node>>& ShuffleChildren() const;

        std::vector<std::shared_ptr<Node>> shuffled_children_;
    
    };
}
