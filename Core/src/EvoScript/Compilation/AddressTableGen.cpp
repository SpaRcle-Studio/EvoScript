//
// Created by Nikita on 09.07.2021.
//

#include <EvoScript/Tools/Debug.h>
#include "EvoScript/Compilation/AddressTableGen.h"

bool EvoScript::AddressTableGen::RegisterClass(
        const std::string &name,
        const std::string &header,
        const std::vector<Property> &properties,
        const std::set<std::string> &includes,
        uint32_t priority)
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
        .m_priority   = priority
    };

    if (auto f = m_headers.find(header); f != m_headers.end()) { // add in exists header
        m_headers[header].m_classes[name] = _class;
        m_headers[header].m_includes      = Tools::Merge<std::string>(m_headers[header].m_includes, includes);
    }
    else { // create new header
        std::map<std::string, Class> m = { { name, _class } };
        m_headers[header] = { includes, m };
    }

    return true;
}

bool EvoScript::AddressTableGen::RegisterMethod(
    void *functionPrt,
    const std::string &className,
    const std::string &methodName,
    const std::string &returnType,
    const std::vector<std::string>& argTypes)
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
            .m_args    = argTypes
        };

        auto& [key, header] = *_class;
        m_headers[header].m_classes[className].m_methods.emplace_back(method);

        return true;
    }
}