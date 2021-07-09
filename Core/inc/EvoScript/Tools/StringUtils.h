//
// Created by Nikita on 08.07.2021.
//

#ifndef EVOSCRIPT_STRINGUTILS_H
#define EVOSCRIPT_STRINGUTILS_H

#include <string>

namespace EvoScript::Tools {
    inline static std::string Read(const std::string& str, uint32_t count) {
        return str.substr(0, count);
    }

    static std::string Replace(std::string str, const std::string& from, const std::string& to) {
    ret:
        size_t start_pos = str.find(from);
        if(start_pos == std::string::npos)
            return str;
        str.replace(start_pos, from.length(), to);
        goto ret;
    }

    static std::string FixPath(const std::string& path) {
        return Tools::Replace(Tools::Replace(path, "\\", "/"), "//", "/");
    }

    static std::string BackReadTo(const std::string& str, const char c) {
        std::string result;

        for (uint32_t i = (str.size() - 1); i > 0; i--) {
            if (str[i] != c)
                result += str[i];
            else
                break;
        }

        std::reverse(result.begin(), result.end());

        return result;
    }
}

#endif //EVOSCRIPT_STRINGUTILS_H
