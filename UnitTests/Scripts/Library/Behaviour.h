//
// Created by Evo Script code generator on Mon May 23 16:54:27 2022 | Author - Monika
//

#ifndef EVOSCRIPTLIB_BEHAVIOUR_H
#define EVOSCRIPTLIB_BEHAVIOUR_H

#ifndef EXTERN
#define EXTERN extern "C" __declspec(dllexport)
#endif

#include <functional>

#include "Standard/BehaviourRegistration.h"

class Behaviour;

class Behaviour : public NonCopyable {
public:
	virtual void Update(float_t dt) { }
	virtual void Awake() { }
	virtual void Close() { }
	virtual void Start() { }
};

#endif