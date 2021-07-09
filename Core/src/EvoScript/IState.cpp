//
// Created by Nikita on 08.07.2021.
//

#include <EvoScript/Win32State.hpp>

EvoScript::IState *EvoScript::IState::Allocate(const std::string &path) {
    #ifdef WIN32
        return new Win32State(path);
    #else
        return nullptr;
    #endif
}
