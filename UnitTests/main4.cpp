//
// Created by Monika on 23.05.2022.
//

#include <EvoScript/Tools/Debug.h>
#include <EvoScript/Compilation/AddressTableGen.h>
#include <EvoScript/Compilation/Compiler.h>

namespace EvoScript {
    class Behaviour : private Tools::NonCopyable {
    public:
        ~Behaviour() override = default;

    };
}

int main() {
    EvoScript::Tools::ESDebug::Log   = [](const std::string& msg) { std::cout << "[LOG] "   << msg << std::endl; };
    EvoScript::Tools::ESDebug::Info  = [](const std::string& msg) { std::cout << "[INFO] "  << msg << std::endl; };
    EvoScript::Tools::ESDebug::Warn  = [](const std::string& msg) { std::cout << "[WARN] "  << msg << std::endl; };
    EvoScript::Tools::ESDebug::Error = [](const std::string& msg) { std::cout << "[ERROR] " << msg << std::endl; };

    auto* address = new EvoScript::AddressTableGen();

    {
        address->RegisterNewClass("Behaviour", "Behaviour",
            { "Standard/BehaviourRegistration.h" },
            {
                EvoScript::InheritClass{ "NonCopyable", EvoScript::Public }
            }
        );

        ESRegisterVirtualMethod(EvoScript::Public, address, Behaviour, Update, void, ESArg1(float_t dt))
        ESRegisterVirtualMethodArg0(EvoScript::Public, address, Behaviour, Awake, void)
        ESRegisterVirtualMethodArg0(EvoScript::Public, address, Behaviour, Close, void)
        ESRegisterVirtualMethodArg0(EvoScript::Public, address, Behaviour, Start, void)
    }

    address->Save(R"(Z:\SREngine\Engine\Core\Dependences\Framework\Depends\EvoScript\UnitTests\Scripts\Library\)");

    auto* compiler = EvoScript::Compiler::Create("Visual Studio 16 2019", R"(Z:\Cache)");
    compiler->SetApiVersion(address->GetApiVersion());

    auto* script = EvoScript::Script::Allocate("Just script", compiler, address->GetAddresses(), true);
    if (!script->Load(R"(Z:\SREngine\Engine\Core\Dependences\Framework\Depends\EvoScript\UnitTests\Scripts\BehaviourTest)", true)) {
        return -1;
    }

    script->Call<EvoScript::Typedefs::InitBehaviourFnPtr>("InitBehaviour");
    script->Call<EvoScript::Typedefs::StartFnPtr>("Start");
    script->Call<EvoScript::Typedefs::UpdateFnPtr>("Update", 0.f);
    script->Call<EvoScript::Typedefs::ReleaseBehaviourFnPtr>("ReleaseBehaviour");

    script->Destroy();
    script->Free();

    compiler->Free();

    return 0;
}