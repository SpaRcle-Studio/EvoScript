//
// Created by Monika on 30.10.2021.
//

#ifndef GAMEENGINE_CASTING_H
#define GAMEENGINE_CASTING_H

#include <string>
#include <functional>
#include <EvoScript/IState.h>
#include <EvoScript/Compilation/Method.h>
#include <EvoScript/Compilation/AddressTableGen.h>

namespace EvoScript {
    enum class CastingType {
        Unknown, Dynamic, Static, Reinterpret
    };

    static std::string CastingTypeToString(CastingType type) {
        switch (type) {
            case CastingType::Dynamic: return "Dynamic";
            case CastingType::Static: return "Static";
            case CastingType::Reinterpret: return "Reinterpret";
            case CastingType::Unknown:
            default:
                return "Unknown";
        }
    }

    class CastingGen {
    public:
        explicit CastingGen(AddressTableGen* addressTableGen) : m_addressTable(addressTableGen) { };
    private:
        std::set<std::string> m_incomplete;
        std::vector<Method> m_casts;
        AddressTableGen* m_addressTable;
    private:
        std::string Generate();
    public:
        bool RegisterCast(
                const std::function<void(EvoScript::IState*)>& setter,
                const std::string& from,
                const std::string& to,
                CastingType type);

        bool Save(const std::string& folder);
    };
}

#define ESRegisterDynamicCast(generator, _nsFrom, _from, _nsTo, _to) {                             \
    auto fun = [] (EvoScript::IState* state) {                                                     \
        typedef void(*SetterFnPtr)(const std::function<_nsTo _to* (_nsFrom _from* from)>&);        \
        const auto name = std::string("DynamicCast")                                               \
            .append(#_from).append("To").append(#_to).append("FnPtrSetter");                       \
        if (auto fun = state->GetFunction<SetterFnPtr>(name.c_str()))                              \
            fun([](_nsFrom _from* from) -> _nsTo _to* { return dynamic_cast<_nsTo _to*>(from); }); \
    };                                                                                             \
    generator->RegisterCast(fun, #_from, #_to, EvoScript::CastingType::Dynamic); }                 \

#endif //GAMEENGINE_CASTING_H
