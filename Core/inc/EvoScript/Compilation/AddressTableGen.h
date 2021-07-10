//
// Created by Nikita on 09.07.2021.
//

#ifndef EVOSCRIPT_ADDRESSTABLEGEN_H
#define EVOSCRIPT_ADDRESSTABLEGEN_H

#include <EvoScript/Tools/SetUtils.h>
#include <EvoScript/Tools/StringUtils.h>
#include <EvoScript/Compilation/EvoClass.h>

namespace EvoScript {
    struct Header {
        std::string                  m_name;
        std::set<std::string>        m_includes;
        std::map<std::string, Class> m_classes;

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

            result += GetIncludes() + "\n";

            for (const auto& _class : m_classes)
                result += _class.second.ToString() += "\n\n";

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
        std::map<std::string, Header> m_headers;
    public:
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
                void* functionPrt,
                const std::string& className,
                const std::string& methodName,
                const std::string& returnType,
                const std::vector<std::string>& argTypes,
                MethodType type);

        bool RegisterClass(
                const std::string& name,
                const std::string& header,
                const std::vector<Property>& properties,
                const std::set<std::string>& includes = {},
                const std::set<InheritClass>& inherit = {});
    };
}

#define ESRegisterVirtualMethod(_addrTable, _class, _method, _returnType, _args) {                  \
    _returnType (_class::*_ptr)_args = &_class::_method;                                            \
    void* pp = *reinterpret_cast<void**>(&_ptr);                                                    \
    std::vector<std::string> strArgs =                                                              \
        EvoScript::Tools::RemoveFirstSpaces(                                                        \
            EvoScript::Tools::Split(                                                                \
                EvoScript::Tools::DeleteSymbolsInStr(#_args, "()")));                               \
    _addrTable->RegisterMethod(pp, #_class, #_method, #_returnType, strArgs, EvoScript::Virtual); } \

#define ESRegisterOverrideMethod(_addrTable, _class, _method, _returnType, _args) {                  \
    _returnType (_class::*_ptr)_args = &_class::_method;                                             \
    void* pp = *reinterpret_cast<void**>(&_ptr);                                                     \
    std::vector<std::string> strArgs =                                                               \
        EvoScript::Tools::RemoveFirstSpaces(                                                         \
            EvoScript::Tools::Split(                                                                 \
                EvoScript::Tools::DeleteSymbolsInStr(#_args, "()")));                                \
    _addrTable->RegisterMethod(pp, #_class, #_method, #_returnType, strArgs, EvoScript::Override); } \

#define ESRegisterMethod(_addrTable, _class, _method, _returnType, _args) {                        \
    _returnType (_class::*_ptr)_args = &_class::_method;                                           \
    void* pp = *reinterpret_cast<void**>(&_ptr);                                                   \
    std::vector<std::string> strArgs =                                                             \
        EvoScript::Tools::RemoveFirstSpaces(                                                       \
            EvoScript::Tools::Split(                                                               \
                EvoScript::Tools::DeleteSymbolsInStr(#_args, "()")));                              \
    _addrTable->RegisterMethod(pp, #_class, #_method, #_returnType, strArgs, EvoScript::Normal); } \

#define ESRegisterStaticMethod(_addrTable, _class, _method, _returnType, _args) {                  \
    _returnType (*_ptr)_args = &_class::_method;                                                   \
    void* pp = *reinterpret_cast<void**>(&_ptr);                                                   \
    std::vector<std::string> strArgs =                                                             \
        EvoScript::Tools::RemoveFirstSpaces(                                                       \
            EvoScript::Tools::Split(                                                               \
                EvoScript::Tools::DeleteSymbolsInStr(#_args, "()")));                              \
    _addrTable->RegisterMethod(pp, #_class, #_method, #_returnType, strArgs, EvoScript::Static); } \

#endif //EVOSCRIPT_ADDRESSTABLEGEN_H
