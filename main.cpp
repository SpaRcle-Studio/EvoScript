#include <iostream>

#include <EvoScript/Compilation/Compiler.h>
#include <EvoScript/Compilation/AddressTableGen.h>

class Parent {
public:
    virtual void PrintHello() { }
    int abc;
};

class SimpleClass : public Parent {
private:
    int number = 20;
public:
    std::string str;
    std::vector<int> v;
public:
    int Summ(int a, int b) {
        return a + b;
    }

    void PrintHello() override {
        std::cout << "Hello\n";
    }

    std::string GetString() {
        return str;
    }

    void Add(int i) {
        v.emplace_back(i);
    }

    std::vector<int> GetVector() { return v; }

    int GetNumber() {
        return number;
    }

    void Print(const char* msg) {
        std::cout << msg << std::endl;
    }

    static void StaticMethod() {
        std::cout << "static method\n";
    }
};

int main() {
    EvoScript::Tools::ESDebug::Log   = [](const std::string& msg) { std::cout << "[LOG] "   << msg << std::endl; };
    EvoScript::Tools::ESDebug::Info  = [](const std::string& msg) { std::cout << "[INFO] "  << msg << std::endl; };
    EvoScript::Tools::ESDebug::Warn  = [](const std::string& msg) { std::cout << "[WARN] "  << msg << std::endl; };
    EvoScript::Tools::ESDebug::Error = [](const std::string& msg) { std::cout << "[ERROR] " << msg << std::endl; };

    auto* address = new EvoScript::AddressTableGen();

    address->RegisterClass("Parent", "Header", {
            { "int", "abc",    EvoScript::Public  }
    });
    ESRegisterVirtualMethod(address, Parent, PrintHello, void, ())

    address->RegisterClass("SimpleClass", "Header", {
        { "int",         "number", EvoScript::Private },
        { "std::string", "str",    EvoScript::Public  }
    },
    {
        "string", "iostream", "vector"
    },
    {
        EvoScript::InheritClass { "Parent", EvoScript::Public }
    });
    ESRegisterOverrideMethod(address, SimpleClass, PrintHello, void, ())
    ESRegisterStaticMethod(address, SimpleClass, StaticMethod, void, ())
    ESRegisterMethod(address, SimpleClass, Print, void, (const char*))
    ESRegisterMethod(address, SimpleClass, Summ, int, (int, int))
    ESRegisterMethod(address, SimpleClass, GetNumber, int, ())
    ESRegisterMethod(address, SimpleClass, GetString, std::string, ())
    ESRegisterMethod(address, SimpleClass, GetVector, std::vector<int>, ())
    ESRegisterMethod(address, SimpleClass, Add, void, (int))

    address->Save(R"(J:\C++\EvoScript\UnitTests\Scripts\Library\)");

    auto* compiler = EvoScript::Compiler::Create("Visual Studio 16 2019", R"(J:\C++\EvoScript\Cache)");

    auto* script = EvoScript::Script::Allocate("Just script", compiler, true);
    if (!script->Load(R"(J:\C++\EvoScript\UnitTests\Scripts\Example)")) {
        return -1;
    }

    typedef void(*ProcessFnPtr)(SimpleClass*);
    auto fun = script->GetState()->GetFunction<ProcessFnPtr>("Process");
    auto simple = new SimpleClass();
    simple->str = "aboba";
    fun(simple);

    script->Awake();
    script->Start();
    script->Update(0.5);
    script->Close();

    script->Destroy();
    script->Free();

    compiler->Free();

    return 0;
}
