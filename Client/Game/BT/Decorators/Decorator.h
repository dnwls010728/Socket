#pragma once
#include "BT/Node.h"

namespace BT
{
    class Decorator : public Node
    {
    public:
        Decorator(const std::wstring& kName);
        virtual ~Decorator() override = default;

        virtual void Reset() override;

        void AddChild(const std::shared_ptr<Node>& kNode);
        
        FORCEINLINE const std::shared_ptr<Node>& GetChild() const { return child_; }

    protected:
        std::shared_ptr<Node> child_;
    
    };
}
