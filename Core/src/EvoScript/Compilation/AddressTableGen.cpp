//
// Created by Nikita on 09.07.2021.
//

#include <EvoScript/Tools/Debug.h>
#include <EvoScript/Tools/FileSystem.h>
#include "EvoScript/Compilation/AddressTableGen.h"

bool EvoScript::AddressTableGen::RegisterNewClass(
        const std::string &name,
        const std::string &header,
        const std::vector<Property> &properties,
        std::set<std::string> includes,
        const std::vector<InheritClass>& inherit)
{
    includes.insert(m_standardLib);

    if (auto f = m_classes.find(name); f != m_classes.end()) {
        ES_ERROR("AddressTableGen::RegisterClass() : class \"" + name + "\" is already exists!");
        return false;
    } else m_classes[name] = header;

    Class _class = {
        .m_header     = header,
        .m_name       = name,
        .m_properties = properties,
        .m_methods    = { },
        .m_inherit    = inherit,
    };

    if (auto f = m_headers.find(header); f != m_headers.end()) { // add in exists header
        m_headers[header].m_classes.emplace_back(_class);
        m_headers[header].m_includes = Tools::Merge<std::string>(m_headers[header].m_includes, includes);
    }
    else { // create new header
        m_headers[header] = { header, includes, { }, { }, { }, { _class } };
    }

    return true;
}

bool EvoScript::AddressTableGen::RegisterMethod(
    void *functionPrt,
    const std::string &className,
    const std::string &methodName,
    const std::string &returnType,
    const std::vector<std::string>& argTypes,
    MethodType type,
    const std::string& _overrideClass,
    Publicity publicity)
{
    if (auto _class = m_classes.find(className); _class == m_classes.end()) {
        ES_ERROR("AddressTableGen::RegisterMethod() : class \"" + className + "\" isn't exists!")
        return false;
    }
    else {
        this->m_methodPointers.emplace_back(functionPrt);

        Method method = {
            .m_id       = (uint32_t)(m_methodPointers.size() - 1),
            .m_name     = methodName,
            .m_class    = className,
            .m_return   = returnType,
            .m_args     = argTypes,
            .m_type     = type,
            .m_override = _overrideClass,
            .m_public   = publicity,
        };

        auto& [key, header] = *_class;
        m_headers[header].FindClass(className)->m_methods.emplace_back(method);

        return true;
    }
}

bool EvoScript::AddressTableGen::Save(const std::string& libFolder) {
    Tools::CreatePath(Tools::FixPath(libFolder));

    if (!GenerateStandardLibrary(libFolder)) {
        return false;
    }

    for (const auto& [key, header] : m_headers) {
        std::string path = libFolder + key + ".h";
        std::ofstream file(path);
        if (!file.is_open()) {
            ES_ERROR("AddressTableGen::Save() : failed to create file! \n\tPath: " + path);
            return false;
        }

        file << header.ToString();

        file.close();
    }

    return true;
}

bool EvoScript::AddressTableGen::GenerateStandardLibrary(const std::string& libFolder) {
    Tools::CreatePath(Tools::FixPath(libFolder + "Standard/"));

    std::string path = libFolder + m_standardLib;
    std::ofstream file(path);
    if (file.is_open()) {
        file << "#ifndef EVOSCRIPTLIB_ADDRESSES_H\n";
        file << "#define EVOSCRIPTLIB_ADDRESSES_H\n\n";

        file << "#define EXTERN extern \"C\" __declspec(dllexport)\n\n";

        file << "void** g_methodPointers; \n\n";

        file << "EXTERN void Init(void** methodPointers) {\n";
        file << "\tg_methodPointers = methodPointers;\n";
        file << "}\n\n";

        file << "#endif\n";
    } else {
        ES_ERROR("AddressTableGen::GenerateStandardLibrary() : failed to open file! \n\tPath: " + path);
        return false;
    }

    file.close();

    return true;
}

bool EvoScript::AddressTableGen::RegisterEnum(
        const std::string &name,
        const std::string &header,
        bool asClass,
        const std::vector<std::pair<std::string, int32_t>>& values)
{
    EvoEnum evoEnum = {
            .m_name = name,
            .m_header = header,
            .m_asClass = asClass,
            .m_values = values
    };

    if (auto f = m_headers.find(header); f != m_headers.end()) // add in exists header
        m_headers[header].m_enums.emplace_back(evoEnum);
    else // create new header
        m_headers[header] = { header, { /* includes */ }, { /* incomplete */ }, { }, { evoEnum }, { } };

    return true;
}

bool EvoScript::AddressTableGen::AddIncompleteType(const std::string &className, const std::string &header) {
    if (auto f = m_headers.find(header); f != m_headers.end()) // add in exists header
        m_headers[header].m_incompleteTypes.emplace_back(className);
    else // create new header
        m_headers[header] = { header, { /* includes */ }, { className }, { }, { }, { } };

    return true;
}

bool EvoScript::AddressTableGen::RegisterTypedef(
        const std::string &name,
        const std::string& header,
        const std::string &value)
{
    std::string _typedef = "typedef " + value + " " + name + ";\n";
    if (auto f = m_headers.find(header); f != m_headers.end()) // add in exists header
        m_headers[header].m_typedefs.emplace_back(_typedef);
    else // create new header
        m_headers[header] = { header, { /* includes */ }, { /*incomplete*/ }, { _typedef }, { /*enums*/ }, { /*classes*/ } };

    return true;
}
