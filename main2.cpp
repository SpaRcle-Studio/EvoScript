//
// Created by Nikita on 11.07.2021.
//

#include <iostream>

#include <EvoScript/Compilation/Compiler.h>
#include <EvoScript/Compilation/AddressTableGen.h>

class A {
public:
    int a = 1;
    std::string lol = "lol";
    std::string lol2 = "lol2";

    void PrintA() {
        std::cout << a << std::endl;
    }
    virtual void OverPrintA() { }
    virtual void VirtualA() {
        std::cout << "virtual" << std::endl;
    }

    static void foo() {

    }
};

class B {
public:
    B(int i) {
        b = i;
    }

    int b = 2;
    int b2 = 2;
    std::string str = "some";

    //static int bar() { return 3; }

    void PrintB() {
        std::cout << b << std::endl;
    }
    virtual void OverPrintB() { }
};

class C : public A, public B {
    int c = 3;
public:
    C() : B(22) { }

    void PrintC() const {
        std::cout << a << std::endl;
        std::cout << b << std::endl;
        std::cout << c << std::endl;
    }
    inline void OverPrintA() override {
        std::cout << str << std::endl;
    }
    void OverPrintB() override {
        //std::cout << "sdsdsdsd " << str << " " << a << b << c << std::endl;
        std::cout << str << std::endl;
    }
};

int main() {
    std::cout << "main2\n";

    EvoScript::Tools::ESDebug::Log   = [](const std::string& msg) { std::cout << "[LOG] "   << msg << std::endl; };
    EvoScript::Tools::ESDebug::Info  = [](const std::string& msg) { std::cout << "[INFO] "  << msg << std::endl; };
    EvoScript::Tools::ESDebug::Warn  = [](const std::string& msg) { std::cout << "[WARN] "  << msg << std::endl; };
    EvoScript::Tools::ESDebug::Error = [](const std::string& msg) { std::cout << "[ERROR] " << msg << std::endl; };

    auto* address = new EvoScript::AddressTableGen();

    address->RegisterNewClass("A", "Header", {
            { "int", "a", EvoScript::Public },
            { "std::string", "lol", EvoScript::Public },
            { "std::string", "lol2", EvoScript::Public },
    });

    ESRegisterMethodArg0(::, EvoScript::Public, address, A, PrintA, void)
    ESRegisterMethodVirtualArg0(::, EvoScript::Public, address, A, OverPrintA, void)
    ESRegisterMethodVirtualArg0(::, EvoScript::Public, address, A, VirtualA, void)

    address->RegisterNewClass("B", "Header", {
            { "int", "b", EvoScript::Public },
            { "int", "b2", EvoScript::Public },
            { "std::string", "str", EvoScript::Public }
    });
    ESRegisterMethodArg0(::, EvoScript::Public, address, B, PrintB, void)
    ESRegisterMethodVirtualArg0(::, EvoScript::Public, address, B, OverPrintB, void)

    address->RegisterNewClass("C", "Header", {
            { "int", "c", EvoScript::Public }
    },
    { "string" }, { { "A", EvoScript::Public }, { "B", EvoScript::Public } });
    ESRegisterMethodArg0(::, EvoScript::Public, address, C, PrintC, void)
    ESRegisterMethodOverrideArg0(::, EvoScript::Public, address, C, OverPrintA, void)
    ESRegisterMethodOverrideArg0(::, EvoScript::Public, address, C, VirtualA, void)
    ESRegisterMethodOverrideArg0(::, EvoScript::Public, address, C, OverPrintB, void)

    address->Save(R"(J:\C++\GameEngine\Engine\Dependences\Framework\Depends\EvoScript\UnitTests\Scripts\Library\)");

#ifdef __MINGW64__
    auto* compiler = EvoScript::Compiler::Create("MinGW Makefiles", R"(J:\C++\EvoScript\Cache)");
#else
    auto* compiler = EvoScript::Compiler::Create("Visual Studio 16 2019", R"(J:\C++\EvoScript\Cache)");
#endif

    auto* script = EvoScript::Script::Allocate("Just script", compiler, address->GetAddresses(), true);
    if (!script->Load(R"(J:\C++\GameEngine\Engine\Dependences\Framework\Depends\EvoScript\UnitTests\Scripts\Example2)")) {
        return -1;
    }

    auto ptr = new C();

    typedef void(*ProcessFnPtr)(C*);
    auto fun = script->GetState()->GetFunction<ProcessFnPtr>("Process");
    fun(ptr);

    script->Destroy();
    script->Free();
    

    compiler->Free();

    return 0;
}
