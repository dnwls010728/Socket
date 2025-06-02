#pragma once
#include "BT/Node.h"

namespace BT
{
    class Composite : public Node
    {
    public:
        Composite(const std::wstring& kName);
        virtual ~Composite() override = default;

        virtual void Reset() override;

        void AddChild(const std::shared_ptr<Node>& kNode);

        FORCEINLINE const std::vector<std::shared_ptr<Node>>& GetChildren() const { return children_; }

    protected:
        std::vector<std::shared_ptr<Node>> children_;
        int32_t current_child_;
    
    };
}
