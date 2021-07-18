//
// Created by Nikita on 09.07.2021.
//

#ifndef EVOSCRIPT_ADDRESSTABLEGEN_H
#define EVOSCRIPT_ADDRESSTABLEGEN_H

#include <EvoScript/Tools/SetUtils.h>
#include <EvoScript/Tools/StringUtils.h>
#include <EvoScript/Compilation/EvoClass.h>

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

            if (std::string inc = GetIncludes(); !inc.empty())
                result +=  inc + "\n";

            for (const auto& incomplete : m_incompleteTypes)
                result += "class " + incomplete + ";\n";
            if (!m_incompleteTypes.empty())
                result += "\n";

            for (const auto& _typedef : m_typedefs)
                result += _typedef;
            if (!m_typedefs.empty())
                result += "\n";

            for (const auto& _enum : m_enums)
                result += _enum.ToString() + "\n\n";

            for (const auto& _class : m_classes)
                result += _class.ToString() + "\n\n";

            result += "#endif";

            return result;
        }
    };

    class AddressTableGen {
    public:
        AddressTableGen()  = default;
        ~AddressTableGen() = default;

        AddressTableGen(const AddressTableGen&) = delete;
    private:
        //! key - class name, value - header name
        std::map<std::string, std::string> m_classes;
        std::map<std::string, Header>      m_headers;
        std::vector<void*>                 m_methodPointers;
        inline static const std::string    m_standardLib = "Standard/Addresses.h";
    public:
        [[nodiscard]] Header GetHeader(const std::string& name) const {
            if (auto f = m_headers.find(name); f == m_headers.end()) {
                ES_ERROR("AddressTableGen::GetHeader() : header isn't exists!");
                return { };
            } else
                return m_headers.at(name);
        }
        bool Save(const std::string& libFolder);
    private:
        static bool GenerateStandardLibrary(const std::string& libFolder);
    public:
        [[nodiscard]] void** GetAddresses() const { return (void**)m_methodPointers.data(); }

        bool RegisterMethod(
                void* functionPrt,
                const std::string& className,
                const std::string& methodName,
                const std::string& returnType,
                const std::vector<std::string>& argTypes,
                MethodType type,
                const std::string& _overrideClass = "",
                Publicity publicity = Publicity::Public);

        bool RegisterNewClass(
                const std::string& name,
                const std::string& header,
                const std::vector<Property>& properties,
                std::set<std::string> includes = {},
                const std::vector<InheritClass>& inherit = {});

        bool RegisterEnum(
                const std::string& name,
                const std::string& header,
                bool asClass,
                const std::vector<std::pair<std::string, int32_t>>& values);

        bool RegisterTypedef(const std::string& name, const std::string& header, const std::string& value);

        bool AddIncompleteType(const std::string& className, const std::string& header);
    };
}

#define ESRegisterVirtualMethod(_ns, _pub, _addrTable, _class, _method, _returnType, _args) {                          \
    _returnType (_ns _class::*_ptr)_args = &_ns _class::_method;                                                       \
    void* pp = *reinterpret_cast<void**>(&_ptr);                                                                       \
    std::vector<std::string> strArgs = EvoScript::Tools::GetArgs(#_args);                                              \
    _addrTable->RegisterMethod(pp, #_class, #_method, #_returnType, strArgs, EvoScript::Virtual, "", _pub); }          \

#define ESRegisterOverrideMethod(_ns, _pub, _addrTable, _class, _method, _returnType, _args, _override) {              \
    _returnType (_ns _class::*_ptr)_args = &_ns _class::_method;                                                       \
    void* pp = *reinterpret_cast<void**>(&_ptr);                                                                       \
    std::vector<std::string> strArgs = EvoScript::Tools::GetArgs(#_args);                                              \
    _addrTable->RegisterMethod(pp, #_class, #_method, #_returnType, strArgs, EvoScript::Override, _override, _pub); }  \

#define ESRegisterVirtualOverrideMethod(_ns, _pub, _addrTable, _class, _method, _returnType, _args, _override) {              \
    _returnType (_ns _class::*_ptr)_args = &_ns _class::_method;                                                              \
    void* pp = *reinterpret_cast<void**>(&_ptr);                                                                              \
    std::vector<std::string> strArgs = EvoScript::Tools::GetArgs(#_args);                                                     \
    _addrTable->RegisterMethod(pp, #_class, #_method, #_returnType, strArgs, EvoScript::VirtualOverride, _override, _pub); }  \

#define ESRegisterMethod(_ns, _pub, _addrTable, _class, _method, _returnType, _args) {                                 \
    _returnType (_ns _class::*_ptr)_args = &_ns _class::_method;                                                       \
    void* pp = *reinterpret_cast<void**>(&_ptr);                                                                       \
    std::vector<std::string> strArgs = EvoScript::Tools::GetArgs(#_args);                                              \
    _addrTable->RegisterMethod(pp, #_class, #_method, #_returnType, strArgs, EvoScript::Normal, "", _pub); }           \

#define ESRegisterStaticMethod(_ns, _pub, _addrTable, _class, _method, _returnType, _args) {                           \
    _returnType (*_ptr)_args = &_ns _class::_method;                                                                   \
    void* pp = *reinterpret_cast<void**>(&_ptr);                                                                       \
    std::vector<std::string> strArgs = EvoScript::Tools::GetArgs(#_args);                                              \
    _addrTable->RegisterMethod(pp, #_class, #_method, #_returnType, strArgs, EvoScript::Static, "", _pub); }           \

#endif //EVOSCRIPT_ADDRESSTABLEGEN_H
