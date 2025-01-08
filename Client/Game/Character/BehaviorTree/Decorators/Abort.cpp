#include "pch.h"
#include "Abort.h"

BT::Abort::Abort(const std::wstring& kName, const Function<bool()>& kFunc) :
    Node(kName),
    func_(kFunc)
{
}

BT::Node::Status BT::Abort::TickNode(float delta_time)
{
    if (func_())
    {
        Reset();
        return Status::kFailure;
    }

    switch (children_[0]->TickNode(delta_time))
    {
    case Status::kRunning:
        {
            return Status::kRunning;
        }

    case Status::kSuccess:
        {
            Reset();
            return Status::kSuccess;
        }
    }

    Reset();
    return Status::kFailure;
}
