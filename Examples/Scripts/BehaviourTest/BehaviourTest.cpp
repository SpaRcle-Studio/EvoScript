//
// Created by Nikita on 08.07.2021.
//

#include "BehaviourTest.h"

REGISTER_BEHAVIOUR(BehaviourTest)

void BehaviourTest::Update(float_t dt) {
    std::cout << "Hello, from behaviour! Dt: " << dt << "\n";
}
