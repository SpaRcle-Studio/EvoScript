//
// Created by Monika on 23.05.2022.
//

#ifndef EVOSCRIPT_NONCOPYABLE_H
#define EVOSCRIPT_NONCOPYABLE_H

#include <EvoScript/macros.h>

namespace EvoScript::Tools {
    class NonCopyable {
    protected:
        constexpr NonCopyable() = default;
        virtual ~NonCopyable() = default;

        NonCopyable(const NonCopyable&) = delete;
        NonCopyable& operator=(const NonCopyable&) = delete;

    };
}

#endif //EVOSCRIPT_NONCOPYABLE_H
