//
// Created by Nikita on 03.07.2021.
//

#ifndef EVOSCRIPT_COMPILER_H
#define EVOSCRIPT_COMPILER_H

#include <EvoScript/Script.h>
#include <EvoScript/Tools/StringUtils.h>
#include <EvoScript/Tools/FileSystem.h>
#include <EvoScript/Tools/HashUtils.h>

namespace EvoScript {
    typedef std::unordered_map<std::string, std::vector<uint32_t>> ModuleCopies;

    class Compiler : public Tools::NonCopyable {
    public:
        explicit Compiler(std::string cachePath);
        ~Compiler() override = default;

    public:
        ES_NODISCARD std::string GetCachePath() const { return m_cachePath; }
        ES_NODISCARD std::string GetAPIVersion() const { return m_apiVersion; }

        ES_NODISCARD std::string GetCompilerPath() const { return m_compilerPath; }
        void SetCompilerPath(const std::string& compilerPath) { m_compilerPath = compilerPath; }

        IState* AllocateState(const std::string& path);

        void SetApiVersion(std::string version);
        void SetMultiInstances(bool enabled) { m_multiInstances = enabled; }

        void AddIncludePath(const std::string& path);

        bool Compile(Script* script);

        void SetCompilePDB(bool value) { m_compilePDB = value; }

        bool TryLoad(Script* script);
        bool Load(Script* script);

        bool LoadState(IState* state);

    private:
        bool ClearModulesCache(const std::string& path);
        uint32_t FindFreeID(const std::string& pathToModule);
        bool CheckSourceHash(const std::string& source, const std::string& pathToScript, bool debug);
        bool CheckApiHash(const std::string& pathToScript, bool debug);

    private:
        std::vector<std::string> m_includes;

        bool m_compilePDB = false;
        bool m_multiInstances = true;

        std::string m_apiVersion = "None";
        std::string m_cachePath  = "None";

        std::recursive_mutex m_mutex;
        ModuleCopies m_moduleCopies;

        std::string m_compilerPath = "";
    };
}

#endif //EVOSCRIPT_COMPILER_H
