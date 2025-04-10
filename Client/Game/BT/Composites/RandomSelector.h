#pragma once
#include "Selector.h"

namespace BT
{
    class RandomSelector : public Selector
    {
    public:
        RandomSelector(const std::wstring& kName);
        virtual ~RandomSelector() override = default;

        virtual Status TickNode(float delta_time) override;

        virtual void Reset() override;

        static std::shared_ptr<RandomSelector> Create(const std::wstring& kName);

    private:
        const std::vector<std::shared_ptr<Node>>& GetShuffledChildren();
        
        std::vector<std::shared_ptr<Node>> ShuffleChildren() const;

        std::vector<std::shared_ptr<Node>> shuffled_children_;
    
    };
}
