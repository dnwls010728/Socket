#pragma once
#include "Function.h"
#include <vector>

template<typename>
class Delegate;

template<typename Ret, typename... Args>
class Delegate<Ret(Args...)>
{
public:
    Delegate() {}

    /*template<typename F, typename = typename std::enable_if<!std::is_same<Function<Ret(Args...)>, typename std::decay<F>::type>::value>::type>
    void Add(F&& func)
    {
        auto temp = std::make_shared<Function<Ret(Args...)>>(std::forward<F>(func));
        functions_.push_back(*temp);
    }

    template<typename M, typename std::enable_if<std::is_class<M>::value>::type* = nullptr>
    void Add(M* target, Ret(M::*func)(Args...))
    {
        auto temp = std::make_shared<Function<Ret(Args...)>>(target, func);
        functions_.push_back(*temp);
    }

    template<typename M, typename std::enable_if<std::is_class<M>::value>::type* = nullptr>
    void Add(M* target, Ret(M::* func)(Args...) const)
    {
        auto temp = std::make_shared<Function<Ret(Args...)>>(target, func);
        functions_.push_back(*temp);
    }

    void Add(Ret(*func)(Args...))
    {
        auto temp = std::make_shared<Function<Ret(Args...)>>(func);
        functions_.push_back(*temp);
    }

    

    template<typename M>
    void Add(void(M::*func)(void), M* m)
    {
        auto temp = std::make_shared<Function<void(void)>>(func, m);
        functions_.push_back(*temp);
    }

    void Add(Ret(*func)(Args...), Args... args)
    {
        auto temp = std::make_shared<Function<Ret(Args...)>>(func, args...);
        functions_.push_back(*temp);
    }

    template<typename M>
    void Add(Ret(M::*func)(Args...), M* m)
    {
        auto temp = std::make_shared<Function<Ret(Args...)>>(func, m);
        functions_.push_back(*temp);
    }*/
    
    template<typename M, typename std::enable_if<std::is_class<M>::value>::type* = nullptr>
    void Add(M* target, Ret(M::*func)(Args...))
    {
        auto temp = std::make_shared<Function<Ret(Args...)>>(target, func);
        functions_.push_back(*temp);
    }

    void Add(Ret(*func)(Args&&...))
    {
        auto temp = std::make_shared<Function<Ret(Args...)>>(func);
        functions_.push_back(*temp);
    }

    template<typename M>
    void Add(M* m, Ret(M::*func)(Args&&...))
    {
        auto temp = std::make_shared<Function<Ret(Args...)>>(func, m);
        functions_.push_back(*temp);
    }

    void Execute(Args&&...args) const
    {
        for (const auto& kTemp : functions_)
        {
            kTemp(std::forward<Args>(args)...);
        }
    }

    /*void Execute() const
    {
        for (const auto& temp : functions_)
        {
            temp();
        }
    }*/

    void RemoveAll()
    {
        functions_.clear();
    }

    template<typename F, typename = typename std::enable_if<!std::is_same<Function<Ret(Args...)>, typename std::decay<F>::type>::value>::type>
    void Remove(F func)
    {
        std::uintptr_t tt = 0;
        std::memcpy(&tt, &func, sizeof(tt));
        auto it = functions_.begin();
        for (auto& temp : functions_)
        {
            if (temp.GetAddr() == tt)
            {
                functions_.erase(it);
                break;
            }
            ++it;
        }
    }

    template<typename M, typename std::enable_if<std::is_class<M>::value>::type* = nullptr>
    void Remove(Ret(M::* func)(Args...))
    {
        std::uintptr_t tt = reinterpret_cast<std::uintptr_t&>(func);
        for (auto temp = functions_.begin(); temp != functions_.end(); ++temp)
        {
            if (temp->GetAddr() == tt)
            {
                functions_.erase(temp);
                break;
            }
        }
    }

    template<typename M, typename std::enable_if<std::is_class<M>::value>::type* = nullptr>
    void Remove(Ret(M::* func)(Args...) const)
    {
        std::uintptr_t tt = reinterpret_cast<std::uintptr_t&>(func);
        for (auto temp = functions_.begin(); temp != functions_.end(); ++temp)
        {
            if (temp->GetAddr() == tt)
            {
                functions_.erase(temp);
                break;
            }
        }
    }

    void Remove(Ret(*func)(Args...))
    {
        std::uintptr_t tt = 0;
        std::memcpy(&tt, &func, sizeof(tt));
        auto it = functions_.begin();
        for (auto& temp : functions_)
        {
            if (temp.GetAddr() == tt)
            {
                functions_.erase(it);
                break;
            }
            ++it;
        }
    }

    template<typename F, typename = typename std::enable_if<!std::is_same<Function<Ret(Args...)>, typename std::decay<F>::type>::value>::type>
    bool IsBound(F&& func)
    {
        std::uintptr_t tt = reinterpret_cast<std::uintptr_t&>(&func);
        for (auto temp = functions_.begin(); temp != functions_.end(); ++temp)
        {
            if (temp->GetAddr() == tt)
            {
                return true;
            }
        }
        return false;
    }

    template<typename M, typename std::enable_if<std::is_class<M>::value>::type* = nullptr>
    bool IsBound(Ret(M::* func)(Args...))
    {
        std::uintptr_t tt = reinterpret_cast<std::uintptr_t&>(func);
        for (auto temp = functions_.begin(); temp != functions_.end(); ++temp)
        {
            if (temp->GetAddr() == tt)
            {
                return true;
            }
        }
        return false;
    }

    template<typename M, typename std::enable_if<std::is_class<M>::value>::type* = nullptr>
    bool IsBound(Ret(M::* func)(Args...) const)
    {
        std::uintptr_t tt = reinterpret_cast<std::uintptr_t&>(func);
        for (auto temp = functions_.begin(); temp != functions_.end(); ++temp)
        {
            if (temp->GetAddr() == tt)
            {
                return true;
            }
        }
        return false;
    }

    bool IsBound(Ret(*func)(Args...))
    {
        std::uintptr_t tt = 0;
        std::memcpy(&tt, &func, sizeof(tt));
        for (auto& temp : functions_)
        {
            if (temp.GetAddr() == tt)
            {
                return true;
            }
        }
        return false;
    }

private:
    std::vector<Function<Ret(Args...)>> functions_;
};

