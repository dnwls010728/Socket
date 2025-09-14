#pragma once
#include "Function.h"
#include <vector>
#include <type_traits>

template <typename Signature>
class Delegate;

template <typename Ret, typename... Args>
class Delegate<Ret(Args...)>
{
public:
    using FunctionType = Function<Ret(Args...)>;
    using Handle = uint64_t;

    Delegate() :
        next_handle_(1)
    {
    }

    template <typename F,
              typename = std::enable_if_t<!std::is_same_v<FunctionType, std::decay_t<F>>>>
    Handle AddLambda(F&& func)
    {
        functions_.emplace_back(Node{next_handle_, FunctionType(std::forward<F>(func))});
        return next_handle_++;
    }

    template <typename M, typename = std::enable_if_t<std::is_class_v<M>>>
    Handle AddObject(M* target, Ret (M::*func)(Args...))
    {
        functions_.emplace_back(Node{next_handle_, FunctionType(target, func)});
        return next_handle_++;
    }

    template <typename M, typename = std::enable_if_t<std::is_class_v<M>>>
    Handle AddObject(M* target, Ret (M::*func)(Args...) const)
    {
        functions_.emplace_back(Node{next_handle_, FunctionType(target, func)});
        return next_handle_++;
    }

    Handle AddStatic(Ret (*func)(Args...))
    {
        functions_.emplace_back(Node{next_handle_, FunctionType(func)});
        return next_handle_++;
    }

    void Execute(Args... args) const
    {
        for (const auto& node : functions_)
        {
            node.function(std::forward<Args>(args)...);
        }
    }

    bool ExecuteIfBound(Args... args) const
    {
        if (!IsBound()) return false;
        Execute(std::forward<Args>(args)...);
        return true;
    }

    void RemoveAll()
    {
        functions_.clear();
    }

    bool Remove(Handle handle)
    {
        auto before = functions_.size();
        functions_.erase(std::remove_if(functions_.begin(), functions_.end(),
                                        [handle](const Node& node) { return node.handle == handle; }),
                         functions_.end());
        return functions_.size() != before;
    }

    bool Remove(Ret (*func)(Args...))
    {
        auto before = functions_.size();
        using GC = Function<Ret(Args...)>::GlobalCallable;
        functions_.erase(std::remove_if(functions_.begin(), functions_.end(),
                                        [func](const Node& node)
                                        {
                                            auto gc = dynamic_cast<GC*>(node.function.callable_.get());
                                            return gc && gc->func_ == func;
                                        }),
                         functions_.end());
        return functions_.size() != before;
    }

    template <typename M>
    bool Remove(M* target, Ret (M::*func)(Args...))
    {
        auto before = functions_.size();
        using MC = typename FunctionType::template MemberCallable<M>;
        functions_.erase(std::remove_if(functions_.begin(), functions_.end(),
                                        [target, func](const Node& node)
                                        {
                                            auto mc = dynamic_cast<MC*>(node.function.callable_.get());
                                            return mc && mc->target_ == target && mc->func_ == func;
                                        }),
                         functions_.end());
        return functions_.size() != before;
    }

    template <typename M>
    bool Remove(M* target, Ret (M::*func)(Args...) const)
    {
        auto before = functions_.size();
        using CMC = typename FunctionType::template ConstMemberCallable<M>;
        functions_.erase(std::remove_if(functions_.begin(), functions_.end(),
                                        [target, func](const Node& node)
                                        {
                                            auto mc = dynamic_cast<CMC*>(node.function.callable_.get());
                                            return mc && mc->target_ == target && mc->func_ == func;
                                        }),
                         functions_.end());
        return functions_.size() != before;
    }

    /**
     * 이 델리게이트가 바인딩된 함수가 있는지 확인합니다.
     * @return 어떤 함수가 바인딩되어 있으면 true, 아니면 false를 반환합니다.
     */
    bool IsBound() const
    {
        return !functions_.empty();
    }

private:
    struct Node
    {
        Handle handle;
        FunctionType function;
    };

    std::vector<Node> functions_;
    Handle next_handle_;
};
