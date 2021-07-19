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
#include <map>
#include <vector>

namespace EvoScript {
    typedef std::map<std::string, std::vector<uint32_t>> ModuleCopies;

    class Compiler {
    public:
        Compiler(const Compiler&) = delete;
    private:
        Compiler() = default;
        ~Compiler() = default;
    private:
        std::string  m_generator     = "None";
        std::string  m_cachePath     = "None";
        std::mutex   m_mutex         = std::mutex();

        ModuleCopies m_moduleCopies  = ModuleCopies();
    private:
        uint32_t FindFreeID(const std::string& pathToModule);
        /*
            true - not need recompile
            false - need recompile
         */
        bool CheckHash(const std::string& source, const std::string& scriptName, bool debug);
    public:
        IState* AllocateState(const std::string& path);

        bool Compile(Script* script);
    public:
        static Compiler* Create(const std::string& generator, const std::string& cachePath);
        void Destroy();
        void Free();
    };
}

#endif //EVOSCRIPT_COMPILER_H
