//
// Created by Nikita on 09.07.2021.
//

#ifndef EVOSCRIPT_RANDOMIZER_H
#define EVOSCRIPT_RANDOMIZER_H

#include <random>

namespace EvoScript::Tools {
    static int32_t Random() {
        std::random_device rd;
        std::mt19937 mersenne(rd()); // инициализируем Вихрь Мерсенна случайным стартовым числом
        return mersenne();
    }
}

#endif //EVOSCRIPT_RANDOMIZER_H
