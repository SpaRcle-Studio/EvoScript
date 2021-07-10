//
// Created by Nikita on 03.07.2021.
//

#include "EvoScript/Compilation/Compiler.h"

#include <EvoScript/Tools/Randomizer.h>

EvoScript::Compiler *EvoScript::Compiler::Create(const std::string& generator, const std::string& cachePath) {
    if (static Compiler* compiler = nullptr; compiler) {
        ES_ERROR("Compiler::Create() : compiler already exists!");
        return nullptr;
    }
    else {
        compiler = new Compiler();

        compiler->m_generator = Tools::FixPath(generator);
        compiler->m_cachePath = Tools::FixPath(cachePath);

        return compiler;
    }
}

void EvoScript::Compiler::Free() {
    delete this;
}

bool EvoScript::Compiler::Compile(EvoScript::Script* script) {
    bool success = false;

    m_mutex.lock();
    {
        auto path   = Tools::FixPath(script->GetPath());
        auto source = path + "-Source";
        auto name   = EvoScript::Tools::BackReadTo(path, '/');
        auto build  = m_cachePath + "/" + name + "-Build/";
        auto module = path + IState::Extension;

        if (Tools::FileExists(module))
            Tools::RemoveFile(module);

        Tools::CreatePath(build);

        ES_LOG("Compiler::Compile() : compile \"" + script->GetName() + "\" script...");

        if (script->IsDebug()) {
            system(std::string("cd " + build + " && "
                               "cmake -G \"" + m_generator + "\" " + source + " && "
                               "cmake --build . --config Debug").c_str());

            if (auto files = Tools::GetAllFilesInDirWithExt(build + "Debug", IState::Extension); files.size() == 1)
                Tools::Copy(files[0], module);
            else
                ES_ERROR("Compiler::Compile() : file not found!");
        }
        else {
            system(std::string("cd " + build + " && "
                               "cmake -G \"" + m_generator + "\" " + source + " && "
                               "cmake --build . --config Release").c_str());
            if (auto files = Tools::GetAllFilesInDirWithExt(build + "Release", IState::Extension); files.size() == 1)
                Tools::Copy(files[0], module);
            else
                ES_ERROR("Compiler::Compile() : file not found!");
        }

        if (success = Tools::FileExists(module); success)
            ES_LOG("Compiler::Compile() : successfully compiled the \"" + script->GetName() + "\" script!")
        else
            ES_ERROR("Compiler::Compile() : failed to compile the \"" + script->GetName() + "\" script!")
    }
    m_mutex.unlock();

    return success;
}

void EvoScript::Compiler::Destroy() {

}

bool EvoScript::Compiler::RegisterScript(EvoScript::Script *script) {
    return false;
}

bool EvoScript::Compiler::RemoveScript(EvoScript::Script *script) {
    return false;
}

EvoScript::IState *EvoScript::Compiler::AllocateState(const std::string &path) {
    auto name = EvoScript::Tools::BackReadTo(Tools::FixPath(path), '/');

ret:
    if (auto find = m_moduleCopies.find(path); find == m_moduleCopies.end()) {
        m_moduleCopies[path] = std::vector<uint32_t>();
        goto ret;
    }
    else {
        auto id = this->FindFreeID(path);
        find->second.emplace_back(id);

        std::string module = m_cachePath + "/Modules/" + name + "-" + std::to_string(id) + IState::Extension;

        Tools::CreateDirectory(m_cachePath + "/Modules");
        Tools::Copy(path + IState::Extension, module);

        return IState::Allocate(module);
    }
}

uint32_t EvoScript::Compiler::FindFreeID(const std::string &pathToModule) {
    if (auto find = m_moduleCopies.find(pathToModule); find == m_moduleCopies.end()) {
        ES_ERROR("Compiler::FindFreeID() : module not found! Something went wrong...");
        return UINT32_MAX;
    } else {
    ret:
        uint32_t id = Tools::Random();

        auto[key, value] { *find };

        for (const auto& copy : value)
            if (id == copy)
                goto ret;

        return id;
    }
}
