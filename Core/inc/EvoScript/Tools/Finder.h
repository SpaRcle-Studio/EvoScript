//
// Created by Monika on 31.12.2022.
//

#ifndef SRENGINE_FINDER_H
#define SRENGINE_FINDER_H

#include <EvoScript/Tools/StringUtils.h>
#include <EvoScript/Tools/Debug.h>
#include <EvoScript/Tools/FileSystem.h>
#include <filesystem>

namespace EvoScript::Tools {
    static const std::vector<std::string> VS_PATHS = {
        "C:/Program Files (x86)/Microsoft Visual Studio",
        "C:/Program Files/Microsoft Visual Studio",

        "D:/Program Files (x86)/Microsoft Visual Studio",
        "D:/Program Files/Microsoft Visual Studio",

        "E:/Program Files (x86)/Microsoft Visual Studio",
        "E:/Program Files/Microsoft Visual Studio",
    };

    static const std::vector<std::string> VS_YEARS = {
            "2015", "2016", "2017", "2018",
            "2019", "2020", "2021", "2022",
            "2023", "2024", "2025", "2026",
            "2027", "2028", "2029", "2030",
    };

    static const std::vector<std::string> VS_TYPES = {
            "Professional", "Community", "Enterprise", "BuildTools"
    };

    static std::string FindMSVCVars64(const std::string& compilerPath) {
        std::filesystem::path path(compilerPath);

        for (const auto& part : path) {
            if (part == "VC") {
                return (path.root_path() / part / "Auxiliary" / "Build" / "vcvars64.bat").string();
            }
        }

        return std::string();
    }

    static std::string FindMSVCVars64() {
        for (auto&& vsPath : VS_PATHS) {
            for (const std::string& yearFolder: ESFileSystem::GetAllFoldersInDir(vsPath)) {
                auto &&pIt = std::find_if(VS_YEARS.begin(), VS_YEARS.end(), [&yearFolder](auto &&element) -> bool {
                    return yearFolder.find(element) != std::string::npos;
                });

                if (pIt == VS_YEARS.end()) {
                    continue;
                }

                for (auto&& typeFolder: ESFileSystem::GetAllFoldersInDir(yearFolder)) {
                    pIt = std::find_if(VS_TYPES.begin(), VS_TYPES.end(), [&typeFolder](auto &&element) -> bool {
                        return typeFolder.find(element) != std::string::npos;
                    });

                    if (pIt == VS_TYPES.end()) {
                        continue;
                    }

                    return typeFolder + "/VC/Auxiliary/Build/vcvars64.bat";
                }
            }
        }

        return std::string();
    }

    static std::string FindMSVC() {
        for (auto&& vsPath : VS_PATHS) {
            for (const std::string& yearFolder: ESFileSystem::GetAllFoldersInDir(vsPath)) {
                auto &&pIt = std::find_if(VS_YEARS.begin(), VS_YEARS.end(), [&yearFolder](auto &&element) -> bool {
                    return yearFolder.find(element) != std::string::npos;
                });

                if (pIt == VS_YEARS.end()) {
                    continue;
                }

                for (auto&& typeFolder: ESFileSystem::GetAllFoldersInDir(yearFolder)) {
                    pIt = std::find_if(VS_TYPES.begin(), VS_TYPES.end(), [&typeFolder](auto &&element) -> bool {
                        return typeFolder.find(element) != std::string::npos;
                    });

                    if (pIt == VS_TYPES.end()) {
                        continue;
                    }

                    for (auto &&versionFolder: ESFileSystem::GetAllFoldersInDir(typeFolder + "/VC/Tools/MSVC")) {
                        return versionFolder;
                    }
                }
            }
        }

        return std::string();
    }

    static std::string FindMSVCCompiler() {
        if (auto&& msvc = FindMSVC(); !msvc.empty()) {
            return msvc.append("/bin/Hostx64/x64/cl.exe");
        }

        return std::string();
    }

    static std::string FindMSVCInclude() {
        if (auto&& msvc = FindMSVC(); !msvc.empty()) {
            msvc += "/include";
            return msvc;
        }

        return std::string();
    }
}

#endif //SRENGINE_FINDER_H
