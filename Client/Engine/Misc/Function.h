#pragma once
#include <memory>
#include <type_traits>

template <typename>
class Delegate;

template <typename>
class Function;

template <typename Ret, typename... Args>
class Function<Ret(Args...)>
{
public:
    Function() = delete;
    ~Function() = default;

    Function(const Function&) = default;
    Function(Function&&) noexcept = default;
    Function& operator=(const Function&) = default;
    Function& operator=(Function&&) noexcept = default;

    template <typename F,
              typename D = std::decay_t<F>,
              typename = std::enable_if_t<!std::is_same_v<Function<Ret(Args...)>, D>>>
    Function(F&& func) :
        callable_(std::make_shared<LambdaCallable<D>>(std::forward<F>(func)))
    {
    }

    Function(Ret (*func)(Args...)) :
        callable_(std::make_shared<GlobalCallable>(func))
    {
    }

    template <typename M>
    Function(M* target, Ret (M::*func)(Args...)) :
        callable_(std::make_shared<MemberCallable<M>>(target, func))
    {
    }

    template <typename M>
    Function(M* target, Ret (M::*func)(Args...) const) :
        callable_(std::make_shared<ConstMemberCallable<M>>(target, func))
    {
    }

    Ret operator()(Args... args) const
    {
        return callable_->Invoke(std::forward<Args>(args)...);
    }

    explicit operator bool() const
    {
        return static_cast<bool>(callable_);
    }

private:
    struct CallableBase
    {
        virtual ~CallableBase() = default;
        virtual Ret Invoke(Args... args) const = 0;
    };

    struct GlobalCallable final : public CallableBase
    {
        explicit GlobalCallable(Ret (*func)(Args...)) :
            func_(func)
        {
        }

        virtual Ret Invoke(Args... args) const override
        {
            return func_(std::forward<Args>(args)...);
        }

        Ret (*func_)(Args...);
    };

    template <typename F>
    struct LambdaCallable final : public CallableBase
    {
        explicit LambdaCallable(F func) :
            func_(std::move(func))
        {
        }

        virtual Ret Invoke(Args... args) const override
        {
            return func_(std::forward<Args>(args)...);
        }

        F func_;
    };

    template <typename M>
    struct MemberCallable final : public CallableBase
    {
        MemberCallable(M* target, Ret (M::*func)(Args...)) :
            target_(target),
            func_(func)
        {
        }

        virtual Ret Invoke(Args... args) const override
        {
            return (target_->*func_)(std::forward<Args>(args)...);
        }

        M* target_;
        Ret (M::*func_)(Args...);
    };

    template <typename M>
    struct ConstMemberCallable final : public CallableBase
    {
        ConstMemberCallable(M* target, Ret (M::*func)(Args...) const) :
            target_(target),
            func_(func)
        {
        }

        virtual Ret Invoke(Args... args) const override
        {
            return (target_->*func_)(std::forward<Args>(args)...);
        }

        M* target_;
        Ret (M::*func_)(Args...) const;
    };

    std::shared_ptr<CallableBase> callable_;

    template <typename>
    friend class Delegate;
};
