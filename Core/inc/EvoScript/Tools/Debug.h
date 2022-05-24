//
// Created by Nikita on 08.07.2021.
//

#ifndef EVOSCRIPT_DEBUG_H
#define EVOSCRIPT_DEBUG_H

#include <EvoScript/macros.h>

namespace EvoScript::Tools {
    class ESDebug {
    public:
        ESDebug()               = delete;
        ~ESDebug()              = delete;
        ESDebug(const ESDebug&) = delete;

    public:
        static inline std::function<void(const std::string& msg)> Log   = std::function<void(const std::string&)>();
        static inline std::function<void(const std::string& msg)> Info  = std::function<void(const std::string&)>();
        static inline std::function<void(const std::string& msg)> Warn  = std::function<void(const std::string&)>();
        static inline std::function<void(const std::string& msg)> Error = std::function<void(const std::string&)>();

    };
}

#define ES_LOG(msg)   EvoScript::Tools::ESDebug::Log(msg);
#define ES_INFO(msg)  EvoScript::Tools::ESDebug::Info(msg);
#define ES_WARN(msg)  EvoScript::Tools::ESDebug::Warn(msg);
#define ES_ERROR(msg) EvoScript::Tools::ESDebug::Error(msg);

#endif //EVOSCRIPT_DEBUG_H
