//
// Created by Nikita on 03.07.2021.
//

#ifndef EVOSCRIPT_ISTATE_H
#define EVOSCRIPT_ISTATE_H

#include <EvoScript/Tools/Debug.h>

namespace EvoScript {
    class IState {
    protected:
        IState(const std::string& path) {
            this->m_path = path;
        }
        ~IState() = default;
    protected:
        //! path to local module copy
        std::string m_path;
    public:
        IState(const IState&) = delete;
    public:
        static IState* Allocate(const std::string& path);
    public:
        [[nodiscard]] std::string GetPath() const { return m_path; }
    public:
#ifdef WIN32
        static const inline std::string Extension = ".dll";
#else
        static const inline std::string Extension = ".UNKNOWN";
#endif
    public:
        template<typename Fn> inline Fn GetFunction(const char* name) {
            return reinterpret_cast<Fn>(GetFunctionImpl(name));
        }
    protected:
        virtual void* GetFunctionImpl(const char* name) = 0;
    public:
        virtual bool Destroy() = 0;
        virtual void Free()    = 0;
        virtual bool Exists()  = 0;
        virtual bool Load()    = 0;
        virtual bool Unload()  = 0;
    };
}

#endif //EVOSCRIPT_ISTATE_H
