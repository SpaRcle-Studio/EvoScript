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

    class Script : private Tools::NonCopyable {
        using MethodPointers = std::vector<std::function<void(EvoScript::IState*)>>;
    private:
        Script(std::string name, Compiler* compiler, MethodPointers&& methodPointers, bool needReCompile)
            : m_name(std::move(name))
            , m_compiler(compiler)
            , m_methodPointers(std::move(methodPointers))
            , m_needReCompile(needReCompile)
        { }

        ~Script() override = default;

    public:
        static Script* Allocate(const std::string& name, Compiler* compiler, MethodPointers methodPointers, bool needReCompile);

        template<typename Fn> inline Fn GetFunction(const std::string& name) {
            return reinterpret_cast<Fn>(m_state->GetFunction<Fn>(name.c_str()));
        }

        template<typename Fn> inline bool HasFunction(const std::string& name) {
            return GetFunction<Fn>(name);
        }

        template<typename Fn, typename Return, typename... Args> inline Return Call(Fn fn, Args... args) {
            return fn(std::forward<Args>(args));
        }

        template<typename Fn, typename... Args> inline void Call(Fn fn, Args... args) {
            fn(std::forward<Args>(args)...);
        }

        template<typename Fn, typename Return> inline Return Call(Fn fn) {
            return fn();
        }

        template<typename Fn, typename Return, typename... Args> inline Return Call(const std::string& name, Args... args) {
            return Call<Fn, Return, Args>(GetFunction<Fn>(name), std::forward<Args>(args));
        }

        template<typename Fn, typename... Args> inline void Call(const std::string& name, Args... args) {
            Call<Fn>(GetFunction<Fn>(name), std::forward<Args>(args)...);
        }

        template<typename Fn, typename Return> inline Return Call(const std::string& name) {
            return Call<Fn, Return>(GetFunction<Fn>(name));
        }

        template<typename Fn> inline void Call(const std::string& name) {
            return Call<Fn, void>(GetFunction<Fn>(name));
        }

    public:
        ES_NODISCARD bool IsDebug()          const { return m_debug; }
        ES_NODISCARD IState* GetState()      const { return m_state; }
        ES_NODISCARD std::string GetPath()   const { return m_path;  }
        ES_NODISCARD std::string GetName()   const { return m_name;  }

        ES_NODISCARD bool Awake() const { ES_CALL_DLL(m_awake) }
        ES_NODISCARD bool Start() const { ES_CALL_DLL(m_start) }
        ES_NODISCARD bool Close() const { ES_CALL_DLL(m_close) }
        ES_NODISCARD bool Update(float dt) const { ES_CALL_DLL_ARGS(m_update, (dt)) }
        ES_NODISCARD bool FixedUpdate() const { ES_CALL_DLL(m_fixed) }
        ES_NODISCARD bool OnGUI() const { ES_CALL_DLL(m_onGUI) }

    public:
        bool Load(const std::string& path, bool compile);
        void Destroy();
        void Free() { delete this; }

    private:
        bool HookFunctions();

    public:
        Typedefs::AwakeFnPtr       m_awake  = nullptr;
        Typedefs::StartFnPtr       m_start  = nullptr;
        Typedefs::CloseFnPtr       m_close  = nullptr;
        Typedefs::UpdateFnPtr      m_update = nullptr;
        Typedefs::FixedUpdateFnPtr m_fixed  = nullptr;
        Typedefs::OnGUIFnPtr       m_onGUI  = nullptr;

    private:
        std::string    m_name           = "None";
        std::string    m_path           = "None";
        Compiler*      m_compiler       = nullptr;
        MethodPointers m_methodPointers = { };
        IState*        m_state          = nullptr;
        bool           m_needReCompile  = false;

#ifdef NDEBUG
        const bool  m_debug         = false;
#else
        const bool  m_debug         = true;
#endif

    };
}

#endif //EVOSCRIPT_SCRIPT_H
