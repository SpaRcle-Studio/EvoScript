//
// Created by Nikita on 11.07.2021.
//

#include <EvoScript/Compilation/Method.h>
#include <EvoScript/Tools/StringUtils.h>

void EvoScript::Method::MathArguments() {
    for (uint32_t i = 0; i < m_args.size(); i++) {
        m_stringArgs  += m_args[i];
        m_argNames += EvoScript::Tools::BackReadTo(m_args[i], ' ');
        if (i + 1 != m_args.size()) {
            m_stringArgs  += ", ";
            m_argNames += ", ";
        }
    }
}

std::string EvoScript::Method::ToString() const  {
    std::string result;
    switch (m_type) {
        case Normal:
            result += m_return + " " + m_name + "(" + m_stringArgs + ") {\n";
            break;
        case Virtual:
            result += "virtual " + m_return + " " + m_name + "(" + m_stringArgs + ") {\n";
            break;
        case Override:
            result += m_return + " " + m_name + "(" + m_stringArgs + ") override {\n";
            break;
        case Static:
            result += "static " + m_return + " " + m_name + "(" + m_stringArgs + ") {\n";
            break;
        case VirtualOverride:
            result += "virtual " + m_return + " " + m_name + "(" + m_stringArgs + ") override {\n";
            break;
    }

    std::string name = m_class + m_name + "FnPtr";

    /*if (m_type == Override || m_type == Normal || m_type == Virtual || m_type == VirtualOverride)
        result += "\treturn (*this.*g_" + m_class + m_name + "FnPtr)(" + m_argNames + ");\n";
    else if (m_type == Static)
        result += "\treturn (*g_" + m_class + m_name + "FnPtr)(" + m_argNames + ");\n";*/

    if (m_type == Static) {
        if (m_args.empty())
            result += "\treturn g_" + m_class + m_name + "FnPtr();\n";
        else
            result += "\treturn g_" + m_class + m_name + "FnPtr(" + m_argNames + ");\n";
    } else {
        if (m_args.empty())
            result += "\treturn g_" + m_class + m_name + "FnPtr(this);\n";
        else
            result += "\treturn g_" + m_class + m_name + "FnPtr(this, " + m_argNames + ");\n";
    }

    result += "}";

    return result;
}

std::string EvoScript::Method::GetTypedef(const std::string& inheritClass) const {
    std::string name = m_class + m_name + "FnPtr";

    /*if (m_type == Override || m_type == Normal || m_type == Virtual || m_type == VirtualOverride) {
        if (m_override.empty()) {
            if (inheritClass.empty())
                return m_return + " (" + m_class + "::*" + name + ")(" + m_stringArgs + ");";
            else
                return m_return + " (" + inheritClass + "::*" + name + ")(" + m_stringArgs + ");";
        } else
            return m_return + " (" + m_override + "::*" + name + ")(" + m_stringArgs + ");";
    } else if (m_type == Static)
        return m_return + " (*" + name + ")(" + m_stringArgs + ");";*/

    if (m_type == Static) {
        if (m_args.empty())
            return "std::function<" + m_return + "()> " + name + ";";
        else
            return "std::function<" + m_return + "(" + m_stringArgs + ")> " + name + ";";
    } else {
        if (m_args.empty())
            return "std::function<" + m_return + "(" + m_class + "*)> " + name + ";";
        else
            return "std::function<" + m_return + "(" + m_class + "*, " + m_stringArgs + ")> " + name + ";";
    }
}

std::string EvoScript::Method::GetSetter() const {
    std::string _typedef = m_class + m_name + "FnPtr";

    if (m_type == Static) {
        if (m_args.empty())
            return "extern \"C\" __declspec(dllexport) void " + _typedef + "Setter(const std::function<" + m_return +
                   +"()>& fnPtr) { \n\tg_" + _typedef + " = fnPtr; \n}\n";
        else
            return "extern \"C\" __declspec(dllexport) void " + _typedef + "Setter(const std::function<" + m_return +
                   +"(" + m_stringArgs + ")>& fnPtr) { \n\tg_" + _typedef + " = fnPtr; \n}\n";
    } else {
        if (m_args.empty())
            return "extern \"C\" __declspec(dllexport) void " + _typedef + "Setter(const std::function<" + m_return +
                   +"(" + m_class + "*)>& fnPtr) { \n\tg_" + _typedef + " = fnPtr; \n}\n";
        else
            return "extern \"C\" __declspec(dllexport) void " + _typedef + "Setter(const std::function<" + m_return +
                   +"(" + m_class + "*, " + m_stringArgs + ")>& fnPtr) { \n\tg_" + _typedef + " = fnPtr; \n}\n";
    }
}
