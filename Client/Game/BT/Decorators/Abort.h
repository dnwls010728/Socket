#pragma once
#include "Decorator.h"

namespace BT
{
    class Abort : public Decorator
    {
    public:
        template<typename F, typename = std::enable_if_t<!std::is_same_v<Function<bool(void)>, std::decay_t<F>>>>
        Abort(const std::wstring& kName, F&& func);

        template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
        Abort(const std::wstring& kName, M* target, bool(M::*func)(void));

        template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
        Abort(const std::wstring& kName, M* target, bool(M::*func)(void) const);

        Abort(const std::wstring& kName, bool(*func)(void));
        
        virtual ~Abort() override = default;

        virtual Status TickNode(float delta_time) override;
        
        template<typename F, typename = std::enable_if_t<!std::is_same_v<Function<bool(void)>, std::decay_t<F>>>>
        static std::shared_ptr<Abort> Create(const std::wstring& kName, F&& func);

        template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
        static std::shared_ptr<Abort> Create(const std::wstring& kName, M* target, bool(M::*func)(void));

        template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
        static std::shared_ptr<Abort> Create(const std::wstring& kName, M* target, bool(M::*func)(void) const);

        static std::shared_ptr<Abort> Create(const std::wstring& kName, bool(*func)(void));

    private:
        Function<bool(void)> condition_;
    
    };

    template <typename F, typename>
    Abort::Abort(const std::wstring& kName, F&& func) 
        : Decorator(kName),
          condition_(std::forward<F>(func))
    {
    }

    template <typename M, typename>
    Abort::Abort(const std::wstring& kName, M* target, bool(M::* func)()) :
        Decorator(kName),
        condition_(target, func)
    {
    }

    template <typename M, typename>
    Abort::Abort(const std::wstring& kName, M* target, bool(M::* func)() const) :
        Decorator(kName),
        condition_(target, func)
    {
    }

    template <typename F, typename>
    std::shared_ptr<Abort> Abort::Create(const std::wstring& kName, F&& func)
    {
        return std::make_shared<Abort>(kName, std::forward<F>(func));
    }

    template <typename M, typename>
    std::shared_ptr<Abort> Abort::Create(const std::wstring& kName, M* target, bool(M::* func)())
    {
        return std::make_shared<Abort>(kName, target, func);
    }

    template <typename M, typename>
    std::shared_ptr<Abort> Abort::Create(const std::wstring& kName, M* target, bool(M::* func)() const)
    {
        return std::make_shared<Abort>(kName, target, func);
    }
}
