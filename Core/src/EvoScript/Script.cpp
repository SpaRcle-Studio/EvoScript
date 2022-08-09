//
// Created by Nikita on 03.07.2021.
//

#include <EvoScript/Script.h>
#include <EvoScript/Compilation/Compiler.h>

bool EvoScript::Script::Load(const std::string &path, Compiler& compiler, bool compile) {
    ES_LOG("Script::Load() : loading script " + path);

    if (m_path = path; compile ? !compiler.Compile(this) : !compiler.Load(this)) {
        ES_ERROR("Script::Load() : failed to compile script!");
        return false;
    }

    if (m_state = compiler.AllocateState(m_name); !m_state) {
        ES_ERROR("Script::Load() : failed to allocate script state!");
        return false;
    }

    if (!compiler.LoadState(m_state)) {
        ES_ERROR("Script::Load() : failed to load script state!");
        return false;
    }

    if (!HookFunctions()) {
        ES_ERROR("Script::Load() : failed to hook functions!");
        return false;
    }

    return true;
}

bool EvoScript::Script::HookFunctions() {
    if (!m_state) {
        ES_ERROR("Script::HookFunction() : state is nullptr!");
        return false;
    }

    for (auto&& m_methodPointer : m_methodPointers) {
        if (m_methodPointer) {
            m_methodPointer(m_state);
        }
        else {
            ES_ERROR("Script::HookFunction() : invalid setter!");
            return false;
        }
    }

    if (auto&& setter = m_state->GetFunction<Typedefs::SetAllocateMemoryFnPtr>("SetAllocateMemoryFnPtr")) {
        setter(&ESAllocateMemory);
    }
    else {
        ES_ERROR("Script::HookFunctions() : function \"SetAllocateMemoryFnPtr\" not found!");
        return false;
    }

    if (auto&& setter = m_state->GetFunction<Typedefs::SetFreeMemoryFnPtr>("SetFreeMemoryFnPtr")) {
        setter(&ESFreeMemory);
    }
    else {
        ES_ERROR("Script::HookFunctions() : function \"SetFreeMemoryFnPtr\" not found!");
        return false;
    }

    return true;
}

EvoScript::Script *EvoScript::Script::Allocate(const std::string &name, MethodPointers methodPointers) {
    return new Script(name, std::move(methodPointers));
}

EvoScript::Script::~Script() {
    if (m_state) {
        m_state->Unload();
        delete m_state;
        m_state = nullptr;
    }
}
