//
// Created by Nikita on 10.07.2021.
//

#ifndef EVOSCRIPT_METHOD_H
#define EVOSCRIPT_METHOD_H

#include <string>
#include <type_traits>
#include <typeinfo>
#include <vector>
#include <map>
#include <sstream> //for std::stringstream

namespace EvoScript {
    enum Publicity {
        Unknown, Public, Private, Protected
    };

    enum MethodType {
        Normal, Virtual, Override, Static
    };

    static std::string PublicityToString(Publicity publicity, bool colon = true) {
        switch (publicity) {
            case Unknown:
                return colon ? "unknown:" : "unknown";
            case Public:
                return colon ? "public:" : "public";
            case Private:
                return colon ? "private:" : "private";
            case Protected:
                return colon ? "protected:" : "protected";
        }
        return "";
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
        MethodType               m_type;

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

            std::string result;
            switch (m_type) {
                case Normal:
                    result += m_return + " " + m_name + "(" + strArgs + ") {\n";
                    break;
                case Virtual:
                    result += "virtual " + m_return + " " + m_name + "(" + strArgs + ") {\n";
                    break;
                case Override:
                    result += m_return + " " + m_name + "(" + strArgs + ") override {\n";
                    break;
                case Static:
                    result += "static " + m_return + " " + m_name + "(" + strArgs + ") {\n";
                    break;
            }

            const void *address = static_cast<const void *>(m_pointer);
            std::stringstream ss;
            ss << address;

            if (m_type == Virtual || m_type == Override || m_type == Normal) {
                result += "\tvoid* voidPtr = reinterpret_cast<void*>(0x" + ss.str() + ");\n";
                result += "\ttypedef " + m_return + " (" + m_class + "::*ClassPtr)(" + strArgs + ");\n";
                result += "\tauto origPtr = *reinterpret_cast<ClassPtr*>(&voidPtr);\n";
                result += "\treturn (*this.*origPtr)(" + argNames + ");\n";
            } else if (m_type == Static) {
                result += "\tvoid* voidPtr = reinterpret_cast<void*>(0x" + ss.str() + ");\n";
                result += "\ttypedef " + m_return + " (*ClassPtr)(" + strArgs + ");\n";
                result += "\tauto origPtr = *reinterpret_cast<ClassPtr*>(&voidPtr);\n";
                result += "\treturn (*origPtr)(" + argNames + ");\n";
            }

            result += "}";

            return result;
        }
    };
}

#endif //EVOSCRIPT_METHOD_H
