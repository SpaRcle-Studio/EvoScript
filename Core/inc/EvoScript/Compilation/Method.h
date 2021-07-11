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
        std::string              m_override; // Метод какого класса переопределяется

        [[nodiscard]] std::string ToString(const std::string& inheritClass) const;
    };
}

#endif //EVOSCRIPT_METHOD_H
