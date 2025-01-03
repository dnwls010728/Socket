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

        void AddChild(Node* child);

        virtual void Reset();

        virtual Status Process();

    protected:
        std::wstring name_;

        std::vector<Node*> children_;

        int current_child_;
    };
}
