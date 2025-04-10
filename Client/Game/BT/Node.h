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

    protected:
        std::wstring name_;
    
    };
}
