//
// Created by Nikita on 08.07.2021.
//

#include <iostream>
#include "../Library/Header.h"

extern "C" __declspec(dllexport) void Process(Class* c) {
    std::cout << c->i << std::endl;
    c->Print(10, 50);
}
