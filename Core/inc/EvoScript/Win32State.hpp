//
// Created by Nikita on 03.07.2021.
//

#ifndef EVOSCRIPT_WIN32STATE_HPP
#define EVOSCRIPT_WIN32STATE_HPP

#include <Windows.h>

#include <EvoScript/Tools/FileSystem.h>
#include <EvoScript/IState.h>

namespace EvoScript {
    class Win32State : public IState {
    public:
        Win32State(const Win32State&) = delete;
        Win32State(const std::string& path) : IState(path) { }
    private:
        ~Win32State() = default;
    private:
        HINSTANCE m_hDLL = nullptr;
    public:
        void* GetFunctionImpl(const char* name) override {
            return reinterpret_cast<void*>(::GetProcAddress(m_hDLL, name));
        }
        bool Destroy() override {
            if (Tools::FileExists(m_path)) {
                Tools::RemoveFile(m_path);
                return true;
            } else
                return false;
        }
        void Free() override {
            delete this;
        }
        bool Unload() override {
            if (!m_hDLL) {
                ES_ERROR("Win32State::Unload() : dll isn't loaded!");
                return false;
            }

            if (!FreeLibrary(m_hDLL)) {
                ES_ERROR("Win32State::Unload() : failed to unload dll!");
                return false;
            }

            return true;
        }
        bool Load() override {
            if (m_hDLL) {
                ES_ERROR("Win32State::Load() : dll is already loaded!");
                return false;
            }

            this->m_hDLL = LoadLibrary(m_path.c_str());
            if (!m_hDLL) {
                ES_ERROR("Win32State::Load() : failed to load dll!")
                return false;
            }

            return true;
        }
        bool Exists() override {
            return false;
        }
    };
}

#endif //EVOSCRIPT_WIN32STATE_HPP
