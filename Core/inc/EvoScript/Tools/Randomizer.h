//
// Created by Nikita on 09.07.2021.
//

#ifndef EVOSCRIPT_RANDOMIZER_H
#define EVOSCRIPT_RANDOMIZER_H

#include <random>
#include <cmath>

namespace EvoScript::Tools {
    static int32_t RandomInt32() {
        std::random_device rd;
        std::mt19937 mersenne(rd()); // инициализируем Вихрь Мерсенна случайным стартовым числом
        return mersenne();
    }

    static int32_t RandomUInt32() {
        std::random_device rd;
        std::mt19937 mersenne(rd()); // инициализируем Вихрь Мерсенна случайным стартовым числом
        return abs((int32_t)mersenne());
    }
}

#endif //EVOSCRIPT_RANDOMIZER_H
