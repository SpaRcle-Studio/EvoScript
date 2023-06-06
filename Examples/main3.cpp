//
// Created by Nikita on 11.07.2021.
//

#include <iostream>

#include <EvoScript/Compilation/Compiler.h>
#include <EvoScript/Compilation/AddressTableGen.h>

class Class {
public:
    Class() {
        i = 34;
    }

    int i;

    void PrintEmpty() {
        std::cout << 3432 << std::endl;
    }

    void Print(int a, int b) {
        std::cout << i << std::endl;
    }
};

int main() {
    std::cout << "main3\n";

    EvoScript::Tools::ESDebug::Log   = [](const std::string& msg) { std::cout << "[LOG] "   << msg << std::endl; };
    EvoScript::Tools::ESDebug::Info  = [](const std::string& msg) { std::cout << "[INFO] "  << msg << std::endl; };
    EvoScript::Tools::ESDebug::Warn  = [](const std::string& msg) { std::cout << "[WARN] "  << msg << std::endl; };
    EvoScript::Tools::ESDebug::Error = [](const std::string& msg) { std::cout << "[ERROR] " << msg << std::endl; };

    auto* address = new EvoScript::AddressTableGen();

    address->RegisterNewClass("Class", "Header", {
            { "int", "i", EvoScript::Public  }
    });
    ESRegisterMethodArg0(::, EvoScript::Public, address, Class, PrintEmpty, void)
    ESRegisterMethod(::, EvoScript::Public, address, Class, Print, void, ESArgs2(int a, int b), ESArgs2(a, b))

    address->Save(R"(J:\C++\GameEngine\Engine\Dependences\Framework\Depends\EvoScript\UnitTests\Scripts\Library\)");

    auto* compiler = EvoScript::Compiler::Create("MinGW Makefiles", R"(J:\C++\EvoScript\Cache)");
    //auto* compiler = EvoScript::Compiler::Create("Visual Studio 16 2019", R"(J:\C++\EvoScript\Cache)");

    auto* script = EvoScript::Script::Allocate("Just script", compiler, address->GetAddresses(), true);
    if (!script->Load(R"(J:\C++\GameEngine\Engine\Dependences\Framework\Depends\EvoScript\UnitTests\Scripts\Test)")) {
        return -1;
    }

    auto* ptr = new Class();

    typedef void(*ProcessFnPtr)(Class*);
    auto fun = script->GetState()->GetFunction<ProcessFnPtr>("Process");
    if (fun)
        fun(ptr);

    script->Destroy();
    script->Free();

    compiler->Free();

    return 0;
}
