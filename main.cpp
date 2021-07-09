#include <iostream>

#include <EvoScript/Compiler.h>

int main() {
    EvoScript::Tools::ESDebug::Log   = [](const std::string& msg) { std::cout << "[LOG] "   << msg << std::endl; };
    EvoScript::Tools::ESDebug::Info  = [](const std::string& msg) { std::cout << "[INFO] "  << msg << std::endl; };
    EvoScript::Tools::ESDebug::Warn  = [](const std::string& msg) { std::cout << "[WARN] "  << msg << std::endl; };
    EvoScript::Tools::ESDebug::Error = [](const std::string& msg) { std::cout << "[ERROR] " << msg << std::endl; };

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
