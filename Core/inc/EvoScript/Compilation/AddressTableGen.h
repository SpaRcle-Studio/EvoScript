//
// Created by Nikita on 09.07.2021.
//

#ifndef EVOSCRIPT_ADDRESSTABLEGEN_H
#define EVOSCRIPT_ADDRESSTABLEGEN_H

#include <string>
#include <type_traits>
#include <typeinfo>
#include <vector>
#include <map>
#include <sstream> //for std::stringstream

#include <EvoScript/Tools/SetUtils.h>
#include <EvoScript/Tools/StringUtils.h>

namespace EvoScript {
    enum Publicity {
        Unknown, Public, Private, Protected
    };

    static std::string PublicityToString(Publicity publicity) {
        switch (publicity) {
            case Unknown:
                return "unknown:";
            case Public:
                return "public:";
            case Private:
                return "private:";
            case Protected:
                return "protected:";
        }
    }

    struct Property {
        std::string m_type;
        std::string m_name;
        Publicity   m_pub;
    };

    struct Method {
        void*                    m_pointer;
        std::string              m_name;
        std::string              m_class;
        std::string              m_return;
        std::vector<std::string> m_args;

        [[nodiscard]] std::string ToString() const {
            std::string strArgs;
            std::string argNames;
            for (uint32_t i = 0; i < m_args.size(); i++) {
                strArgs  += m_args[i] + " arg" + std::to_string(i);
                argNames += "arg" + std::to_string(i);
                if (i + 1 != m_args.size()) {
                    strArgs  += ", ";
                    argNames += ", ";
                }
            }

            std::string result = m_return + " " + m_name + "(" + strArgs + ") {\n";

            const void * address = static_cast<const void*>(m_pointer);
            std::stringstream ss; ss << address;

            result += "\tvoid* voidPtr = reinterpret_cast<void*>(0x" + ss.str() + ");\n";
            result += "\ttypedef " + m_return + " (" + m_class + "::*ClassPtr)(" + strArgs + ");\n";
            result += "\tauto origPtr = *reinterpret_cast<ClassPtr*>(&voidPtr);\n";
            result += "\treturn (*this.*origPtr)(" + argNames + ");\n";
            result += "}";

            return result;
        }
    };

    struct Class {
        std::string           m_header;
        std::string           m_name;
        std::vector<Property> m_properties;
        std::vector<Method>   m_methods;
        uint32_t              m_priority;

        [[nodiscard]] std::string ToString() const {
            std::string result;

            result += "class " + m_name + " {\n";

            Publicity currentPub = Unknown;
            for (const Property& property : m_properties) {
                if (currentPub != property.m_pub) {
                    currentPub = property.m_pub;
                    result += PublicityToString(currentPub) + "\n";
                }

                result += "\t" + property.m_type + " " + property.m_name + ";\n";
            }

            result += PublicityToString(Publicity::Public) + "\n";

            for (const auto& method : m_methods)
                result += Tools::TabString(method.ToString()) + "\n";

            result += "};";

            return result;
        }
    };

    struct Header {
        std::set<std::string>        m_includes;
        std::map<std::string, Class> m_classes;

        std::string GetIncludes() {
            std::string result;
            for (const auto& inc : m_includes)
                result += "#include \"" + inc + "\"";
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
    public:
        bool RegisterMethod(
                void* functionPrt,
                const std::string& className,
                const std::string& methodName,
                const std::string& returnType,
                const std::vector<std::string>& argTypes);

        bool RegisterClass(
                const std::string& name,
                const std::string& header,
                const std::vector<Property>& properties,
                const std::set<std::string>& includes = {},
                uint32_t priority = 0);
    };
}

#endif //EVOSCRIPT_ADDRESSTABLEGEN_H
