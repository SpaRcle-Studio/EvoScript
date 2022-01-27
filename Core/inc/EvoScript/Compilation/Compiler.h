//
// Created by Nikita on 03.07.2021.
//

#ifndef EVOSCRIPT_COMPILER_H
#define EVOSCRIPT_COMPILER_H

#include <EvoScript/Script.h>
#include <EvoScript/Tools/StringUtils.h>
#include <EvoScript/Tools/FileSystem.h>
#include <EvoScript/Tools/HashUtils.h>

#include <mutex>
#include <unordered_map>
#include <vector>

namespace EvoScript {
    typedef std::unordered_map<std::string, std::vector<uint32_t>> ModuleCopies;

    class Compiler {
    public:
        Compiler(const Compiler&) = delete;

    private:
        Compiler() = default;
        ~Compiler() = default;

    public:
        IState* AllocateState(const std::string& path);

        void SetApiVersion(const std::string& version);

        bool Compile(Script* script);
        static Compiler* Create(const std::string& generator, const std::string& cachePath);

        void Destroy();
        void Free();

    private:
        bool ClearModulesCache(const std::string& path);
        uint32_t FindFreeID(const std::string& pathToModule);
        bool CheckSourceHash(const std::string& source, const std::string& pathToScript, bool debug);
        bool CheckApiHash(const std::string& pathToScript, bool debug);

    private:
        std::string  m_apiVersion    = "None";
        std::string  m_generator     = "None";
        std::string  m_cachePath     = "None";
        std::mutex   m_mutex         = std::mutex();

        ModuleCopies m_moduleCopies  = ModuleCopies();

    };
}

#endif //EVOSCRIPT_COMPILER_H
