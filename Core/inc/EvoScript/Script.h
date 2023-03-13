//
// Created by Nikita on 03.07.2021.
//

#ifndef EVOSCRIPT_SCRIPT_H
#define EVOSCRIPT_SCRIPT_H

#include <EvoScript/IState.h>
#include <EvoScript/Compilation/Typedefs.h>
#include <EvoScript/Tools/NonCopyable.h>

namespace EvoScript {
    class Compiler;

    static void* ESAllocateMemory(size_t sz) {
        return new char[sz];
    }

    static void ESFreeMemory(void* ptr) {
        delete[] (char*)ptr;
    }

    class Script : private Tools::NonCopyable {
        using MethodPointers = std::vector<std::function<void(EvoScript::IState*)>>;
    private:
        Script(std::string name, Compiler* pCompiler, MethodPointers&& methodPointers)
            : m_name(std::move(name))
            , m_methodPointers(std::move(methodPointers))
            , m_compiler(pCompiler)
        { }

    public:
        ~Script() override;

    public:
        ES_NODISCARD bool IsDirty() const;

        static Script* Allocate(const std::string& name, Compiler* pCompiler, MethodPointers methodPointers);

        template<typename Fn> ES_INLINE Fn GetFunction(const std::string& name) {
            return reinterpret_cast<Fn>(m_state->GetFunction<Fn>(name.c_str()));
        }

        template<typename Fn> ES_INLINE bool HasFunction(const std::string& name) {
            return GetFunction<Fn>(name);
        }

        template<typename Fn, typename Return, typename... Args> ES_INLINE Return Call(Fn fn, Args... args) {
            return fn(std::forward<Args>(args)...);
        }

        template<typename Fn, typename... Args> ES_INLINE void Call(Fn fn, Args... args) {
            fn(std::forward<Args>(args)...);
        }

        template<typename Fn, typename Return> ES_INLINE Return Call(Fn fn) {
            return fn();
        }

        template<typename Fn, typename Return, typename... Args> ES_INLINE Return Call(const std::string& name, Args... args) {
            return Call<Fn, Return, Args...>(GetFunction<Fn>(name), std::forward<Args>(args)...);
        }

        template<typename Fn, typename... Args> ES_INLINE void Call(const std::string& name, Args... args) {
            Call<Fn>(GetFunction<Fn>(name), std::forward<Args>(args)...);
        }

        template<typename Fn, typename Return> ES_INLINE Return Call(const std::string& name) {
            return Call<Fn, Return>(GetFunction<Fn>(name));
        }

        template<typename Fn> ES_INLINE void Call(const std::string& name) {
            return Call<Fn, void>(GetFunction<Fn>(name));
        }

    public:
        ES_NODISCARD bool IsDebug()          const { return m_debug; }
        ES_NODISCARD IState* GetState()      const { return m_state; }
        ES_NODISCARD std::string GetPath()   const { return m_path;  }
        ES_NODISCARD std::string GetName()   const { return m_name;  }

    public:
        bool Load(const std::string& path, Compiler& compiler, bool compile);

    private:
        bool HookFunctions();

    private:
        std::string    m_name           = "None";
        std::string    m_path           = "None";
        MethodPointers m_methodPointers = { };
        IState*        m_state          = nullptr;
        Compiler*      m_compiler       = nullptr;

#ifdef NDEBUG
        const bool  m_debug         = false;
#else
        const bool  m_debug         = true;
#endif

    };
}

#endif //EVOSCRIPT_SCRIPT_H
