//
// Created by Nikita on 03.07.2021.
//

#include "EvoScript/Compilation/Compiler.h"

#include <EvoScript/Tools/Randomizer.h>
#include <iostream>

bool EvoScript::Compiler::ClearModulesCache(const std::string& path) {
    const auto dirs = Tools::GetAllDirsInDir(path);

    /// предполагается, что dll файлы лежат в конкретной директории, и там кроме них нет ничего больше
    if (!dirs.empty()) {
        for (const auto &dir : dirs) {
            if (!ClearModulesCache(dir))
                return false;

            if (Tools::IsDirectoryEmpty(dir))
                Tools::RemoveFolder(dir);
        }

        return true;
    }

    for (const auto& file : Tools::GetAllFilesInDirWithExt(path, IState::Extension)) {
        /// проверка, на случай, если попали в иную директорию, чтобы не удалить пол системы
        if (file.find("Module-") == std::string::npos) {
            ES_WARN("Compiler::ClearModulesCache() : a suspicious file has been detected! I can't perform automatic cache cleanup! \n\tFile: " + file);
            return false;
        }
        Tools::RemoveFile(file);
    }

    return true;
}

bool EvoScript::Compiler::CheckApiHash(const std::string &pathToScript, bool debug) {
    const std::string fullPath = pathToScript + "/api.hash";

    auto currHash = std::pair(debug, std::vector<std::string>{ m_apiVersion });

    if (Tools::FileExists(fullPath)) {
        auto loadHash = Tools::LoadHashInfo(fullPath);
        if (Tools::HashEquals(loadHash, currHash))
            return true;
        else {
            Tools::SaveHashInfo(fullPath, currHash);
        }
    }
    else {
        Tools::SaveHashInfo(fullPath, currHash);
    }

    return false;
}

bool EvoScript::Compiler::CheckSourceHash(const std::string& source, const std::string& pathToScript, bool debug) {
    const std::string fullPath = pathToScript + "/source.hash";

    auto currHash = std::pair(debug, Tools::GetHashAllFilesInDir(source));

    if (Tools::FileExists(fullPath)) {
        auto loadHash = Tools::LoadHashInfo(fullPath);
        if (Tools::HashEquals(loadHash, currHash))
            return true;
        else {
            Tools::SaveHashInfo(fullPath, currHash);
        }
    }
    else {
        Tools::SaveHashInfo(fullPath, currHash);
    }

    return false;
}

bool EvoScript::Compiler::Compile(EvoScript::Script* script) {
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    if (m_apiVersion == "None" || m_apiVersion.empty()) {
        ES_ERROR("Compiler::Compile() : api hash is not set!");
        return false;
    }

    bool success = false;

    {
        const auto path = m_cachePath + "/Scripts/" + Tools::FixPath(script->GetName());
        const auto source = Tools::FixPath(script->GetPath());

        const auto name   = EvoScript::Tools::BackReadTo(script->GetName(), '/');
        const auto build  = path + "/Build/";
        const auto module = path + "/Module" + IState::Extension;

        const bool isDebug = script->IsDebug();

        Tools::CreatePath(path);
        Tools::CreatePath(build);

        /// чтобы все хеш суммы сразу создались, нужно их проверить
        uint32_t hashCompare = static_cast<uint32_t>(CheckSourceHash(source, path, isDebug))
                             + static_cast<uint32_t>(CheckApiHash(path, isDebug));

        if (hashCompare == 2) {
            if (TryLoad(script))
                return true;
        }
        else
            Tools::RemoveFile(module);

        for (const auto& file : Tools::GetAllFilesInDirWithExt(build + (script->IsDebug() ? "Debug" : "Release"), IState::Extension))
            Tools::RemoveFile(file);

        ES_LOG("Compiler::Compile() : compile \"" + script->GetName() + "\" script...");

        if (script->IsDebug()) {
            system(std::string("cd " + build + " && "
                               "cmake -G \"" + m_generator + "\" " + source + " && "
                               "cmake --build . --config Debug").c_str());
        }
        else {
            system(std::string("cd " + build + " && "
                               "cmake -G \"" + m_generator + "\" " + source + " && "
                               "cmake --build . --config Release").c_str());
        }

        std::string postfix;
        if (m_generator.find("Visual Studio") != std::string::npos)
            postfix = script->IsDebug() ? "/Debug" : "/Release";

        if (auto files = Tools::GetAllFilesInDirWithExt(build + postfix, IState::Extension); files.size() == 1)
            Tools::Copy(files[0], module);
        else
            ES_ERROR("Compiler::Compile() : file not found! \n\tPath: " + module);

        if (success = Tools::FileExists(module); success)
            ES_LOG("Compiler::Compile() : successfully compiled the \"" + script->GetName() + "\" script!")
        else
            ES_ERROR("Compiler::Compile() : failed to compile the \"" + script->GetName() + "\" script!")
    }

    return success;
}

EvoScript::IState *EvoScript::Compiler::AllocateState(const std::string &name) {
ret:
    if (auto find = m_moduleCopies.find(name); find == m_moduleCopies.end()) {
        m_moduleCopies[name] = std::vector<uint32_t>();
        goto ret;
    }
    else {
        auto id = this->FindFreeID(name);
        find->second.emplace_back(id);

        const std::string module = m_cachePath + "/Scripts/" + name + "/Module" + IState::Extension;
        const std::string copy = m_cachePath + "/Modules/" + name + "/Module-" + std::to_string(id) + IState::Extension;

        Tools::CreatePath(m_cachePath + "/Modules/" + name);
        if (!Tools::Copy(module, copy)) {
            ES_ERROR("Compiler::AllocateState() : failed to copy file! \n\tSource:" + module + "\n\tDestination: " + copy);
            return nullptr;
        }

        return IState::Allocate(copy);
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

void EvoScript::Compiler::SetApiVersion(std::string version) {
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    m_apiVersion = std::move(version);
}

void EvoScript::Compiler::SetGenerator(std::string generator) {
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    m_generator = std::move(generator);
}

bool EvoScript::Compiler::Load(Script* script) {
    if (!TryLoad(script)) {
        ES_ERROR("Compiler::Load() : failed to load the \"" + script->GetName() + "\" script!");
        return false;
    }

    return true;
}

bool EvoScript::Compiler::TryLoad(EvoScript::Script *script) {
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    const auto path = m_cachePath + "/Scripts/" + Tools::FixPath(script->GetName());
    const auto module = path + "/Module" + IState::Extension;

    if (Tools::FileExists(module)) {
        ES_LOG("Compiler::Load() : successfully loaded the \"" + script->GetName() + "\" script!");
        return true;
    }

    return false;
}

bool EvoScript::Compiler::LoadState(IState* state) {
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    return state->Load();
}

EvoScript::Compiler::Compiler(std::string cachePath)
    : m_cachePath(std::move(cachePath))
{
    ClearModulesCache(m_cachePath + "/Modules");
}
