//
// Created by Nikita on 08.07.2021.
//

#include <iostream>
#include "../Library/Header.h"

EXTERN void Process(C* c) {
    c->PrintA();
    c->PrintB();
    c->PrintC();
    c->OverPrintA();
    c->OverPrintB();

    static_cast<A*>(c)->VirtualA();
}
