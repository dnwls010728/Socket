#include "pch.h"
#include "RandomSelector.h"

#include <random>

BT::RandomSelector::RandomSelector(const std::wstring& kName) :
    Selector(kName),
    shuffled_children_()
{
}

BT::Node::Status BT::RandomSelector::TickNode(float delta_time)
{
    for (const auto& child : GetShuffledChildren())
    {
        switch (child->TickNode(delta_time))
        {
        case Status::kRunning: return Status::kRunning;
        case Status::kSuccess:
            {
                Reset();
                return Status::kSuccess;
            }

        default: continue;
        }
    }
    
    Reset();
    return Status::kFailure;
}

void BT::RandomSelector::Reset()
{
    shuffled_children_.clear();
    
    Selector::Reset();
}

std::shared_ptr<BT::RandomSelector> BT::RandomSelector::Create(const std::wstring& kName)
{
    return std::make_shared<RandomSelector>(kName);
}

const std::vector<std::shared_ptr<BT::Node>>& BT::RandomSelector::GetShuffledChildren()
{
    if (shuffled_children_.empty()) shuffled_children_ = ShuffleChildren();
    return shuffled_children_;
}

std::vector<std::shared_ptr<BT::Node>> BT::RandomSelector::ShuffleChildren() const
{
    std::random_device rd;
    std::mt19937 g(rd());
    
    std::vector<std::shared_ptr<Node>> shuffled_children = children_;
    std::ranges::shuffle(shuffled_children, g);
    
    return shuffled_children;
}
