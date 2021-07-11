#include <iostream>

#include <EvoScript/Compilation/Compiler.h>
#include <EvoScript/Compilation/AddressTableGen.h>

namespace NS {
    class Parent {
    public:
        int abc{};
        std::string str32;

        virtual void PrintHello() {}
    };
}

class SimpleClass : public NS::Parent {
private:
    int number{};
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

class Pointer {
public:
    SimpleClass* ptr = nullptr;
public:
    std::string GetString() {
        return ptr->GetString();
    }

    void Print(const char* str) {
        ptr->Print(str);
    }
};

int main() {
    EvoScript::Tools::ESDebug::Log   = [](const std::string& msg) { std::cout << "[LOG] "   << msg << std::endl; };
    EvoScript::Tools::ESDebug::Info  = [](const std::string& msg) { std::cout << "[INFO] "  << msg << std::endl; };
    EvoScript::Tools::ESDebug::Warn  = [](const std::string& msg) { std::cout << "[WARN] "  << msg << std::endl; };
    EvoScript::Tools::ESDebug::Error = [](const std::string& msg) { std::cout << "[ERROR] " << msg << std::endl; };

    auto* address = new EvoScript::AddressTableGen();

    address->RegisterEnum("SomeEnum", "Header", true, {
            { "first", 0 },
            { "second", 1 },
            { "third", 2 },
    });

    address->RegisterNewClass("Pointer", "Header", {
            { "void*", "self", EvoScript::Private }
    });
    ESRegisterMethod(::, address, Pointer, GetString, std::string, ())
    ESRegisterMethod(::, address, Pointer, Print, void, (const char*))

    address->RegisterNewClass("Parent", "Header", {
            {"int",         "abc",   EvoScript::Public},
            {"std::string", "str32", EvoScript::Public},
    });
    ESRegisterVirtualMethod(NS::, address, Parent, PrintHello, void, ())

    address->RegisterNewClass("SimpleClass", "Header", {
        { "int",         "number", EvoScript::Private },
        { "std::string", "str",    EvoScript::Public  },
        { "std::vector<int>", "v",    EvoScript::Public  },
    },
    {
        "string", "iostream", "vector"
    },
    {
        EvoScript::InheritClass { "Parent", EvoScript::Public },
    });
    ESRegisterMethod(::, address, SimpleClass, Summ, int, (int, int))
    ESRegisterOverrideMethod(::, address, SimpleClass, PrintHello, void, (), "Parent")
    ESRegisterMethod(::, address, SimpleClass, GetString, std::string, ())
    ESRegisterMethod(::, address, SimpleClass, Add, void, (int))
    ESRegisterMethod(::, address, SimpleClass, GetVector, std::vector<int>, ())
    ESRegisterMethod(::, address, SimpleClass, GetNumber, int, ())
    ESRegisterMethod(::, address, SimpleClass, Print, void, (const char*))
    ESRegisterStaticMethod(::, address, SimpleClass, StaticMethod, void, ())

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

    auto* ptr = new Pointer();
    ptr->ptr = simple;
    typedef void(*Process2FnPtr)(Pointer*);
    auto fun2 = script->GetState()->GetFunction<Process2FnPtr>("Process2");
    fun2(ptr);

    script->Awake();
    script->Start();
    script->Update(0.5);
    script->Close();

    script->Destroy();
    script->Free();

    compiler->Free();

    return 0;
}
