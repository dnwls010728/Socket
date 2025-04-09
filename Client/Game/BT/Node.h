#pragma once

namespace BT
{
    class Node
    {
    public:
        enum class Status
        {
            kSuccess,
            kFailure,
            kRunning
        };

        Node(const std::wstring& kName);
        virtual ~Node() = default;

        virtual Status TickNode(float delta_time);
        virtual void Reset();

        void AddChild(const std::shared_ptr<Node>& kNode);

    protected:
        std::wstring name_;
        std::vector<std::shared_ptr<Node>> children_;
        int current_child_;
    
    };
}
