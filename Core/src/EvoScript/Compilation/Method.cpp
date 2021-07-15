//
// Created by Nikita on 11.07.2021.
//

#include <EvoScript/Compilation/Method.h>

std::string EvoScript::Method::ToString(const std::string& inheritClass) const  {
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
        case VirtualOverride:
            result += "virtual " + m_return + " " + m_name + "(" + strArgs + ") override {\n";
            break;
    }

    const void *address = static_cast<const void *>(m_pointer);
    std::stringstream ss;
    ss << address;

    if (m_type == Override || m_type == Normal || m_type == Virtual || m_type == VirtualOverride) {
        result += "\tvoid* voidPtr = reinterpret_cast<void*>(0x" + ss.str() + ");\n";
        if (m_override.empty()) {
            if (inheritClass.empty())
                result += "\ttypedef " + m_return + " (" + m_class + "::*ClassPtr)(" + strArgs + ");\n";
            else
                result += "\ttypedef " + m_return + " (" + inheritClass + "::*ClassPtr)(" + strArgs + ");\n";
        }
        else
            result += "\ttypedef " + m_return + " (" + m_override + "::*ClassPtr)(" + strArgs + ");\n";
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
