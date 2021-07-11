//
// Created by Nikita on 08.07.2021.
//

#include <iostream>
#include "../Library/Header.h"

#define EXTERN_DLL_EXPORT extern "C" __declspec(dllexport)

EXTERN_DLL_EXPORT void Process(C* c) {
    c->PrintA();
    c->PrintB();
    c->PrintC();
    c->OverPrintA();
    c->OverPrintB();

    static_cast<A*>(c)->VirtualA();
}
