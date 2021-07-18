//
// Created by Nikita on 09.07.2021.
//

#ifndef EVOSCRIPT_TYPEDEFS_H
#define EVOSCRIPT_TYPEDEFS_H

namespace EvoScript::Typedefs {
    typedef void(*InitFnPtr)(void**);

    typedef void(*AwakeFnPtr)();
    typedef void(*StartFnPtr)();
    typedef void(*CloseFnPtr)();
    typedef void(*UpdateFnPtr)(float dt);
    typedef void(*FixedUpdateFnPtr)();
}

#endif //EVOSCRIPT_TYPEDEFS_H
