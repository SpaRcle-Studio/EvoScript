//
// Created by Nikita on 03.07.2021.
//

#ifndef EVOSCRIPT_ISTATE_H
#define EVOSCRIPT_ISTATE_H

#include <EvoScript/Tools/Debug.h>
#include <EvoScript/Tools/NonCopyable.h>

namespace EvoScript {
    class IState : private Tools::NonCopyable {
    protected:
        explicit IState(std::string path)
            : m_path(std::move(path))
        { }
        ~IState() override = default;

    public:
        static IState* Allocate(const std::string& path);

        template<typename Fn> inline Fn GetFunction(const char* name) {
            return reinterpret_cast<Fn>(GetFunctionImpl(name));
        }

    public:
        ES_NODISCARD std::string GetPath() const { return m_path; }

        virtual bool Destroy() = 0;
        virtual void Free()    = 0;
        virtual bool Exists()  = 0;
        virtual bool Load()    = 0;
        virtual bool Unload()  = 0;

    protected:
        virtual void* GetFunctionImpl(const char* name) = 0;

    public:
#ifdef WIN32
        static const inline std::string Extension = ".dll";
#else
        static const inline std::string Extension = ".UNKNOWN";
#endif

    protected:
        //! path to local module copy
        std::string m_path;

    };
}

#endif //EVOSCRIPT_ISTATE_H
