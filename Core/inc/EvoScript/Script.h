//
// Created by Nikita on 03.07.2021.
//

#ifndef EVOSCRIPT_SCRIPT_H
#define EVOSCRIPT_SCRIPT_H

#include <EvoScript/IState.h>
#include <EvoScript/Compilation/Typedefs.h>

#include <utility>

#define ES_CALL_DLL(function) \
    if (function) { function(); return true; } else return false;

#define ES_CALL_DLL_ARGS(function, args) \
    if (function) { function args; return true; } else return false;

namespace EvoScript {
    class Compiler;

    class Script {
    public:
        Script(const Script&) = delete;
    private:
        Script(std::string name, Compiler* compiler, std::vector<std::function<void(EvoScript::IState*)>> methodPointers, bool needReCompile)
            : m_name(std::move(name))
            , m_compiler(compiler)
            , m_methodPointers(std::move(methodPointers))
            , m_needReCompile(needReCompile)
        { }

        ~Script() = default;

    public:
        Typedefs::AwakeFnPtr       m_awake  = nullptr;
        Typedefs::StartFnPtr       m_start  = nullptr;
        Typedefs::CloseFnPtr       m_close  = nullptr;
        Typedefs::UpdateFnPtr      m_update = nullptr;
        Typedefs::FixedUpdateFnPtr m_fixed  = nullptr;
        Typedefs::OnGUIFnPtr       m_onGUI  = nullptr;
    private:
        bool HookFunctions();
    public:
        [[nodiscard]] bool IsDebug()          const { return m_debug;      }
        [[nodiscard]] IState* GetState()      const { return m_state;      }
        [[nodiscard]] std::string GetPath()   const { return m_path;       }
        [[nodiscard]] std::string GetName()   const { return m_name;       }

    public:
        bool Awake() const { ES_CALL_DLL(m_awake) }
        bool Start() const { ES_CALL_DLL(m_start) }
        bool Close() const { ES_CALL_DLL(m_close) }
        bool Update(float dt) const { ES_CALL_DLL_ARGS(m_update, (dt)) }
        bool FixedUpdate() const { ES_CALL_DLL(m_fixed) }
        bool OnGUI() const { ES_CALL_DLL(m_onGUI) }

    public:
        static Script* Allocate(
            const std::string& name,
            Compiler* compiler,
            const std::vector<std::function<void(EvoScript::IState*)>>& methodPointers,
            bool needReCompile)
        {
            return new Script(name, compiler, methodPointers, needReCompile);
        }
    public:
        bool Load(const std::string& path, bool compile);
        void Destroy();
        void Free() { delete this; }

    private:
        std::string                                          m_name           = "None";
        std::string                                          m_path           = "None";
        bool                                                 m_needReCompile  = false;
        Compiler*                                            m_compiler       = nullptr;
        std::vector<std::function<void(EvoScript::IState*)>> m_methodPointers = { };

        IState*                                              m_state          = nullptr;

#ifdef NDEBUG
        const bool  m_debug         = false;
#else
        const bool  m_debug         = true;
#endif
    };
}

#endif //EVOSCRIPT_SCRIPT_H
