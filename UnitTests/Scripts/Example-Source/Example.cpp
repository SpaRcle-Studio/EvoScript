//
// Created by Nikita on 08.07.2021.
//

#include <iostream>

#define EXTERN_DLL_EXPORT extern "C" __declspec(dllexport)

EXTERN_DLL_EXPORT void Awake() {
    std::cout << "AWAKE" << std::endl;
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
