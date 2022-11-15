//
// Created by Monika on 23.05.2022.
//

#ifndef EVOSCRIPT_MACROS_H
#define EVOSCRIPT_MACROS_H

#include <utility>
#include <functional>
#include <string>
#include <vector>
#include <mutex>
#include <unordered_map>

#define ES_FUNCTION std::function

#define ES_CALL_DLL(function) \
    if (function) { function(); return true; } else return false;

#define ES_CALL_DLL_ARGS(function, args) \
    if (function) { function args; return true; } else return false;

#define ES_NODISCARD [[nodiscard]]
#define ES_INLINE inline

#ifdef WIN32
    #define ES_WIN32
#endif

#endif //EVOSCRIPT_MACROS_H
