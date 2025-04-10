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

    protected:
        std::vector<std::shared_ptr<Node>> children_;
        int current_child_;
    
    };
}
