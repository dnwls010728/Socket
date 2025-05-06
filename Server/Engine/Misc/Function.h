#pragma once
#include <memory>
#include <type_traits>
#include <cstdint>
#include <cstring>

template<typename>
class Delegate;

template<typename>
class Function;

template<typename Ret, typename... Args>
class Function<Ret(Args...)>
{
public:
    Function() = delete;
    virtual ~Function() = default;

    template<typename F, typename = std::enable_if_t<!std::is_same_v<Function, std::decay_t<F>>>>
    Function(F&& func)
        : callable_(std::make_shared<LambdaCallable<std::decay_t<F>>>(std::forward<F>(func))), addr_(reinterpret_cast<std::uintptr_t>(&func)) {}

    Function(Ret(*func)(Args...))
        : callable_(std::make_shared<GlobalCallable>(func)), addr_(reinterpret_cast<std::uintptr_t>(func)) {}

    template<typename M>
    Function(M* target, Ret(M::*func)(Args...))
        : callable_(std::make_shared<MemberCallable<M>>(target, func)), addr_(reinterpret_cast<std::uintptr_t&>(func)) {}

    template<typename M>
    Function(M* target, Ret(M::*func)(Args...) const)
        : callable_(std::make_shared<ConstMemberCallable<M>>(target, func)), addr_(0) {
        std::memcpy(&addr_, &func, sizeof(addr_));
    }

    Ret operator()(Args... args) const {
        return (*callable_)(std::forward<Args>(args)...);
    }

    void operator=(const Function& other) {
        addr_ = other.addr_;
        callable_ = other.callable_;
    }

    std::uintptr_t GetAddr() const { return addr_; }

private:
    struct CallableBase {
        virtual ~CallableBase() = default;
        virtual Ret operator()(Args&&... args) const = 0;
    };

    struct GlobalCallable : public CallableBase {
        explicit GlobalCallable(Ret(*func)(Args...)) : func_(func) {}
        Ret operator()(Args&&... args) const override {
            return func_(std::forward<Args>(args)...);
        }
        Ret(*func_)(Args...);
    };

    template<typename F>
    struct LambdaCallable : public CallableBase {
        explicit LambdaCallable(F&& func) : func_(std::move(func)) {}
        Ret operator()(Args&&... args) const override {
            return func_(std::forward<Args>(args)...);
        }
        F func_;
    };

    template<typename M>
    struct MemberCallable : public CallableBase {
        MemberCallable(M* target, Ret(M::*func)(Args...))
            : target_(target), func_(func) {}
        Ret operator()(Args&&... args) const override {
            return (target_->*func_)(std::forward<Args>(args)...);
        }
        M* target_;
        Ret(M::*func_)(Args...);
    };

    template<typename M>
    struct ConstMemberCallable : public CallableBase {
        ConstMemberCallable(M* target, Ret(M::*func)(Args...) const)
            : target_(target), func_(func) {}
        Ret operator()(Args&&... args) const override {
            return (target_->*func_)(std::forward<Args>(args)...);
        }
        M* target_;
        Ret(M::*func_)(Args...) const;
    };

    std::shared_ptr<CallableBase> callable_;
    std::uintptr_t addr_;

    template<typename>
    friend class Delegate;
};
