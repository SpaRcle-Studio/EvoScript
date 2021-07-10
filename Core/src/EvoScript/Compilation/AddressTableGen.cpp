//
// Created by Nikita on 09.07.2021.
//

#include <EvoScript/Tools/Debug.h>
#include <EvoScript/Tools/FileSystem.h>
#include "EvoScript/Compilation/AddressTableGen.h"

bool EvoScript::AddressTableGen::RegisterClass(
        const std::string &name,
        const std::string &header,
        const std::vector<Property> &properties,
        const std::set<std::string> &includes,
        const std::set<InheritClass>& inherit)
{
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
        m_headers[header].m_classes[name] = _class;
        m_headers[header].m_includes      = Tools::Merge<std::string>(m_headers[header].m_includes, includes);
    }
    else { // create new header
        std::map<std::string, Class> m = { { name, _class } };
        m_headers[header] = { header, includes, m };
    }

    return true;
}

bool EvoScript::AddressTableGen::RegisterMethod(
    void *functionPrt,
    const std::string &className,
    const std::string &methodName,
    const std::string &returnType,
    const std::vector<std::string>& argTypes,
    Virtualization _virtual)
{
    if (auto _class = m_classes.find(className); _class == m_classes.end()) {
        ES_ERROR("AddressTableGen::RegisterMethod() : class \"" + className + "\" isn't exists!");
        return false;
    }
    else {
        Method method = {
            .m_pointer = functionPrt,
            .m_name    = methodName,
            .m_class   = className,
            .m_return  = returnType,
            .m_args    = argTypes,
            .m_virtual = _virtual
        };

        auto& [key, header] = *_class;
        m_headers[header].m_classes[className].m_methods.emplace_back(method);

        return true;
    }
}

bool EvoScript::AddressTableGen::Save(const std::string& libFolder) {
    Tools::CreatePath(Tools::FixPath(libFolder));

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
