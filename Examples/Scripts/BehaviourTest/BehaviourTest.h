//
// Created by Monika on 23.05.2022.
//

#ifndef EVOSCRIPT_BEHAVIOURTEST_H
#define EVOSCRIPT_BEHAVIOURTEST_H

#include "../Library/Behaviour.h"

class BehaviourTest : public Behaviour {
public:
    ~BehaviourTest() override = default;

public:
    void Update(float_t dt) override;

};

#endif //EVOSCRIPT_BEHAVIOURTEST_H
