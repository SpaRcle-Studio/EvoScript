//
// Created by Nikita on 09.07.2021.
//

#ifndef EVOSCRIPT_TYPEDEFS_H
#define EVOSCRIPT_TYPEDEFS_H

namespace EvoScript::Typedefs {
    /// сборник разных типов методов на все случаи жизни

    typedef void(*InitFnPtr)(void**);

    typedef void(*InitBehaviourFnPtr)();
    typedef void(*ReleaseBehaviourFnPtr)();
    typedef void(*VoidFnPtr)();
    typedef void(*AwakeFnPtr)();
    typedef void(*StartFnPtr)();
    typedef void(*CloseFnPtr)();
    typedef void(*UpdateFnPtr)(float dt);
    typedef void(*FixedUpdateFnPtr)();
    typedef void(*OnGUIFnPtr)();
}

#endif //EVOSCRIPT_TYPEDEFS_H
