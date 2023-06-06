//
// Created by Nikita on 08.07.2021.
//

#include <iostream>
#include "../Library/Header.h"

#define EXTERN_DLL_EXPORT extern "C" __declspec(dllexport)

static inline SimpleClass* g_simpleClass = nullptr;

EXTERN_DLL_EXPORT void Process(SimpleClass* simpleClass) {
    g_simpleClass = simpleClass;

    simpleClass->Print("Lol");
    simpleClass->PrintHello();
    std::cout << simpleClass->Summ(1, 5) << std::endl;
    std::cout << simpleClass->GetNumber() << std::endl;
    std::cout << simpleClass->str << std::endl;
    simpleClass->str = "amogus";
    std::cout << simpleClass->GetString() << std::endl;
}


EXTERN_DLL_EXPORT void Process2(Pointer* ptr) {
    std::cout << ptr->GetString() << std::endl;
    ptr->Print("POINTER");
}

EXTERN_DLL_EXPORT void Awake() {
    std::cout << "AWAKE" << std::endl;

    g_simpleClass->Add(40);
    auto v = g_simpleClass->GetVector();
    std::cout << v[0] << std::endl;

    SimpleClass::StaticMethod();
}

EXTERN_DLL_EXPORT void Start() {
    std::cout << "START" << std::endl;
}

EXTERN_DLL_EXPORT void Update(float dt) {
    std::cout << "UPDATE " << dt << std::endl;
}

EXTERN_DLL_EXPORT void Close() {
    std::cout << "CLOSE" << std::endl;
}
