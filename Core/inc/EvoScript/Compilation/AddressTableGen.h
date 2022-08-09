//
// Created by Nikita on 09.07.2021.
//

#ifndef EVOSCRIPT_ADDRESSTABLEGEN_H
#define EVOSCRIPT_ADDRESSTABLEGEN_H

#include <EvoScript/Tools/SetUtils.h>
#include <EvoScript/Tools/StringUtils.h>
#include <EvoScript/Compilation/EvoClass.h>
#include <EvoScript/IState.h>

namespace EvoScript {
    struct EvoEnum {
        std::string m_name;
        std::string m_header;
        bool        m_asClass;
        std::vector<std::pair<std::string, int32_t>> m_values;

        [[nodiscard]] std::string ToString() const {
            std::string result = "enum ";

            if (m_asClass)
                result += "class ";
            result += m_name + " {\n\t";

            for (const auto& [name, value] : m_values)
                result += name + " = " + std::to_string(value) + ", ";

            result += "\n};";

            return result;
        }
    };

    struct Header {
        std::string              m_name;
        std::set<std::string>    m_includes;
        std::vector<std::string> m_incompleteTypes;
        std::vector<std::string> m_typedefs;
        std::vector<EvoEnum>     m_enums;
        std::vector<Class>       m_classes;
        std::vector<Method>      m_functions;

        Class* FindClass(const std::string& name) {
            for (auto& _class : m_classes)
                if (_class.m_name == name)
                    return &_class;
            return nullptr;
        }

        [[nodiscard]] std::string GetIncludes() const {
            std::string result;
            for (const auto& inc : m_includes)
                result += "#include \"" + inc + "\"\n";
            return result;
        }

        [[nodiscard]] std::string ToString() const {
            std::string result;

            result += "//\n// Created by Evo Script code generator on "
                      + Tools::GetData() + " | Author - Monika\n//\n\n";

            result += "#ifndef EVOSCRIPTLIB_" + Tools::ToUpper(m_name) + "_H\n";
            result += "#define EVOSCRIPTLIB_" + Tools::ToUpper(m_name) + "_H\n\n";

            result += "#ifndef EXTERN\n";
            result += "#define EXTERN extern \"C\" __declspec(dllexport)\n";
            result += "#endif\n\n";

            result += "#include <functional>\n\n";

            if (std::string inc = GetIncludes(); !inc.empty())
                result +=  inc + "\n";

            for (const auto& _enum : m_enums)
                result += _enum.ToString() + "\n\n";

            for (const auto& incomplete : m_incompleteTypes)
                result += "class " + incomplete + ";\n";
            if (!m_incompleteTypes.empty())
                result += "\n";

            for (const auto& _class : m_classes)
                result += _class.GetAsIncomplete() + "\n";
            result += "\n";

            for (const auto& _typedef : m_typedefs)
                result += _typedef;
            if (!m_typedefs.empty())
                result += "\n";

            for (const auto& _class : m_classes) {
                for (const auto &_method : _class.m_methods) {
                    if (_method.m_type == Virtual) {
                        continue;
                    }

                    result += _method.GetSetterWithVar().append("\n");
                }
            }

            for (const auto& _class : m_classes)
                result += _class.ToString() + "\n\n";

            for (auto&& function : m_functions) {
                result += function.GetSetterWithVar().append("\n");
                result += function.ToString();
            }

            result += "#endif";

            return result;
        }
    };

    class AddressTableGen {
        using SetterFn = std::function<void(EvoScript::IState*)>;
    public:
        AddressTableGen()  = default;
        ~AddressTableGen() = default;

        AddressTableGen(const AddressTableGen&) = delete;

    public:
        [[nodiscard]] size_t GetApiHash() const { return m_hash; }
        [[nodiscard]] std::string GetApiVersion() const { return std::to_string(GetApiHash()); }
        [[nodiscard]] std::vector<std::function<void(EvoScript::IState*)>> GetAddresses() const { return m_methodPointers; }
        [[nodiscard]] Header GetHeader(const std::string& name) const {
            if (auto f = m_headers.find(name); f == m_headers.end()) {
                ES_ERROR("AddressTableGen::GetHeader() : header isn't exists!");
                return { };
            } else
                return m_headers.at(name);
        }
        bool Save(const std::string& libFolder);

    public:
        bool RegisterMethod(
                const SetterFn& setter,
                const std::string& className,
                const std::string& methodName,
                const std::string& returnType,
                const std::vector<std::string>& argTypes,
                MethodType type,
                const std::string& _overrideClass = "",
                Publicity publicity = Publicity::Public);

        bool RegisterFunction(
                const SetterFn& setter,
                const std::string& methodName,
                const std::string& returnType,
                const std::vector<std::string>& argTypes,
                const std::string& header);

        bool RegisterMethod(
                const std::string& className,
                const std::string& methodName,
                const std::string& returnType,
                const std::vector<std::string>& argTypes,
                MethodType type,
                const std::string& _overrideClass = "",
                Publicity publicity = Publicity::Public);

        bool RegisterHeader(const std::string& name, const std::set<std::string>& includes = {});

        bool RegisterNewClass(
                const std::string& name,
                const std::string& header,
                const std::set<std::string>& includes = {},
                const std::vector<InheritClass>& inherit = {});

        bool RegisterEnum(
                const std::string& name,
                const std::string& header,
                bool asClass,
                const std::vector<std::pair<std::string, int32_t>>& values);

        bool RegisterTypedef(const std::string& name, const std::string& header, const std::string& value);
        bool AddIncompleteType(const std::string& className, const std::string& header);
        void AddMethodPointer(const std::function<void(EvoScript::IState*)>& setter);
        void HashCombine(const std::string& hashString);

    private:
        //! key - class name, value - header name
        std::map<std::string, std::string>                   m_classes;
        std::map<std::string, Header>                        m_headers;
        std::vector<std::function<void(EvoScript::IState*)>> m_methodPointers;
        size_t                                               m_hash = 0;

    };
}

#define ESArg1(a1) a1
#define ESArg2(a1, a2) a1, a2
#define ESArg3(a1, a2, a3) a1, a2, a3
#define ESArg4(a1, a2, a3, a4) a1, a2, a3, a4
#define ESArg5(a1, a2, a3, a4, a5) a1, a2, a3, a4, a5
#define ESArg6(a1, a2, a3, a4, a5, a6) a1, a2, a3, a4, a5, a6
#define ESArg7(a1, a2, a3, a4, a5, a6, a7) a1, a2, a3, a4, a5, a6, a7

#define ESRegisterMethodArg0(_pub, _addrTable, _class, _method, _returnType) {                                   \
    auto fun = [] (EvoScript::IState* state) {                                                                   \
        typedef void(*SetterFnPtr)(const ES_FUNCTION<_returnType (_class*)>&);                                   \
        auto fun = state->GetFunction<SetterFnPtr>(                                                              \
            std::string(#_class).append(#_method).append("FnPtrSetter").c_str());                                \
        if (fun) fun([](_class* ptr) -> _returnType { return ptr->_method(); });                                 \
    };                                                                                                           \
    _addrTable->RegisterMethod(fun, #_class, #_method, #_returnType, {}, EvoScript::Normal, "", _pub); }         \

#define ESRegisterMethod(_pub, _addrTable, _class, _method, _returnType, _args, _names) {                        \
    std::vector<std::string> strArgs = EvoScript::Tools::GetArgs(#_args);                                        \
    auto fun = [] (EvoScript::IState* state) {                                                                   \
        typedef void(*SetterFnPtr)(const ES_FUNCTION<_returnType (_class*, _args)>&);                            \
        auto fun = state->GetFunction<SetterFnPtr>(                                                              \
            std::string(#_class).append(#_method).append("FnPtrSetter").c_str());                                \
        if (fun) fun([](_class* ptr, _args) -> _returnType { return ptr->_method(_names); });                    \
    };                                                                                                           \
    _addrTable->RegisterMethod(fun, #_class, #_method, #_returnType, strArgs, EvoScript::Normal, "", _pub); }    \

#define ESRegisterVirtualMethodArg0(_pub, _addrTable, _class, _method, _returnType) {                            \
    _addrTable->RegisterMethod(#_class, #_method, #_returnType, {}, EvoScript::Virtual, "", _pub); }             \

#define ESRegisterVirtualMethod(_pub, _addrTable, _class, _method, _returnType, _args) {                         \
    std::vector<std::string> strArgs = EvoScript::Tools::GetArgs(#_args);                                        \
    _addrTable->RegisterMethod(#_class, #_method, #_returnType, strArgs, EvoScript::Virtual, "", _pub); }        \

#define ESRegisterStaticMethodArg0(_pub, _addrTable, _class, _method, _returnType) {                             \
    auto fun = [] (EvoScript::IState* state) {                                                                   \
        typedef void(*SetterFnPtr)(const ES_FUNCTION<_returnType ()>&);                                          \
        auto fun = state->GetFunction<SetterFnPtr>(                                                              \
            std::string(#_class).append(#_method).append("FnPtrSetter").c_str());                                \
        if (fun) fun([]() -> _returnType { return _class::_method(); });                                         \
    };                                                                                                           \
    _addrTable->RegisterMethod(fun, #_class, #_method, #_returnType, {}, EvoScript::Static, "", _pub); }         \

#define ESRegisterStaticMethod(_pub, _addrTable, _class, _method, _returnType, _args, _names) {                  \
    std::vector<std::string> strArgs = EvoScript::Tools::GetArgs(#_args);                                        \
    auto fun = [] (EvoScript::IState* state) {                                                                   \
        typedef void(*SetterFnPtr)(const ES_FUNCTION<_returnType (_args)>&);                                     \
        auto fun = state->GetFunction<SetterFnPtr>(                                                              \
            std::string(#_class).append(#_method).append("FnPtrSetter").c_str());                                \
        if (fun) fun([](_args) -> _returnType { return _class::_method(_names); });                              \
    };                                                                                                           \
    _addrTable->RegisterMethod(fun, #_class, #_method, #_returnType, strArgs, EvoScript::Static, "", _pub); }    \

#define ESRegisterCustomStaticMethod(_pub, _addrTable, _class, _method, _returnType, _args, _function) {         \
    std::vector<std::string> strArgs = EvoScript::Tools::GetArgs(#_args);                                        \
    auto fun = [] (EvoScript::IState* state) {                                                                   \
        typedef void(*SetterFnPtr)(const ES_FUNCTION<_returnType (_args)>&);                                     \
        auto fun = state->GetFunction<SetterFnPtr>(                                                              \
            std::string(#_class).append(#_method).append("FnPtrSetter").c_str());                                \
        if (fun) fun([](_args) -> _returnType { _function });                                                    \
    };                                                                                                           \
    _addrTable->RegisterMethod(fun, #_class, #_method, #_returnType, strArgs, EvoScript::Static, "", _pub); }    \

#define ESRegisterStaticFunc(_addrTable, _method, _returnType, _args, header, _function) {                       \
    std::vector<std::string> strArgs = EvoScript::Tools::GetArgs(#_args);                                        \
    auto fun = [] (EvoScript::IState* state) {                                                                   \
        typedef void(*SetterFnPtr)(const ES_FUNCTION<_returnType (_args)>&);                                     \
        auto fun = state->GetFunction<SetterFnPtr>(                                                              \
            std::string(#_method).append("FnPtrSetter").c_str());                                                \
        if (fun) fun([](_args) -> _returnType { _function });                                                    \
    };                                                                                                           \
    _addrTable->RegisterFunction(fun, #_method, #_returnType, strArgs, header); }                                \

#endif //EVOSCRIPT_ADDRESSTABLEGEN_H
