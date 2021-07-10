#include <iostream>

#include <EvoScript/Compilation/Compiler.h>
#include <EvoScript/Compilation/AddressTableGen.h>

class SimpleClass {
private:
    int number;
public:
    std::string str;
public:
    int Summ(int a, int b) {
        return a + b;
    }

    int GetNumber() {
        return number;
    }

    void Print(const char* msg) {
        std::cout << msg << std::endl;
    }
};

#define ESRegisterMethod(_addrTable, _class, _method, _returnType, _args) {     \
    _returnType (_class::*_ptr)_args = &_class::_method;                        \
    void* pp = *reinterpret_cast<void**>(&_ptr);                                \
    std::vector<std::string> strArgs =                                          \
        EvoScript::Tools::RemoveFirstSpaces(                                    \
            EvoScript::Tools::Split(                                            \
                EvoScript::Tools::DeleteSymbolsInStr(#_args, "()")));           \
    _addrTable->RegisterMethod(pp, #_class, #_method, #_returnType, strArgs); } \

int main() {
    EvoScript::Tools::ESDebug::Log   = [](const std::string& msg) { std::cout << "[LOG] "   << msg << std::endl; };
    EvoScript::Tools::ESDebug::Info  = [](const std::string& msg) { std::cout << "[INFO] "  << msg << std::endl; };
    EvoScript::Tools::ESDebug::Warn  = [](const std::string& msg) { std::cout << "[WARN] "  << msg << std::endl; };
    EvoScript::Tools::ESDebug::Error = [](const std::string& msg) { std::cout << "[ERROR] " << msg << std::endl; };

    auto* address = new EvoScript::AddressTableGen();

    address->RegisterClass("SimpleClass", "Header.h", {
        { "int",         "number", EvoScript::Private },
        { "std::string", "str",    EvoScript::Public  }
    },
    {
        "string", "iostream"
    });
    ESRegisterMethod(address, SimpleClass, Print, void, (const char*))
    ESRegisterMethod(address, SimpleClass, Summ, int, (int, int))

    auto code = address->GetHeader("Header.h").m_classes["SimpleClass"].ToString();
    std::cout << code << std::endl;
    typedef void (SimpleClass::*ClassPtr)(const char* arg0);
    auto* compiler = EvoScript::Compiler::Create(
            R"(F:\Programs\CLion 2020.1\bin\cmake\win\bin\cmake.exe)",
            "Visual Studio 16 2019",
            R"(J:\C++\EvoScript\Cache)");

    auto* script = EvoScript::Script::Allocate("Just script", compiler, true);
    if (!script->Load(R"(J:\C++\EvoScript\UnitTests\Scripts\Example)")) {
        return -1;
    }

    script->Awake();
    script->Start();
    script->Update(0.5);
    script->Close();

    script->Destroy();
    script->Free();

    compiler->Free();

    return 0;
}
