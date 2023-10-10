//
// Created by Nikita on 08.07.2021.
//

#ifndef EVOSCRIPT_FILESYSTEM_H
#define EVOSCRIPT_FILESYSTEM_H

#include <EvoScript/Tools/StringUtils.h>
#include <EvoScript/Tools/Debug.h>

namespace EvoScript::Tools {
    class ESFileSystem {
        using DeleteFn = std::function<bool(const std::string& path)>;
        using IsExistsFn = std::function<bool(const std::string& path)>;
        using CreateFolderFn = std::function<bool(const std::string& path)>;
        using CopyFn = std::function<bool(const std::string& from, const std::string& to)>;
        using GetInDirFn = std::function<std::vector<std::string>(const std::string& path)>;
    public:
        ESFileSystem() = delete;
        ~ESFileSystem() = delete;
        ESFileSystem(const ESFileSystem&) = delete;

    public:
        static inline CreateFolderFn CreateFolder = CreateFolderFn();
        static inline CopyFn Copy = CopyFn();
        static inline IsExistsFn IsExists = IsExistsFn();
        static inline DeleteFn Delete = DeleteFn();
        static inline GetInDirFn GetAllInDir = GetInDirFn();
        static inline GetInDirFn GetAllFilesInDir = GetInDirFn();
        static inline GetInDirFn GetAllFoldersInDir = GetInDirFn();

    };
}

namespace EvoScript::Tools {
    static bool IsDirectoryEmpty(const std::string& path) {
        return ESFileSystem::GetAllInDir(path).empty();
    }

    static std::vector<std::string> GetAllFilesInDirWithExt(const std::string& path, const std::string& ext) {
        auto files = std::vector<std::string>();

        auto all = ESFileSystem::GetAllFilesInDir(path);

        for (const std::string& file : all) {
            if (auto id = file.rfind('.'); id != std::string::npos) {
                if (file.substr(id, file.size() - id) != ext) {
                    continue;
                }
                files.emplace_back(file);
            }
        }

        return files;
    }

    //! path must be a fix
    static void CreatePath(std::string path, uint32_t offset = 0) {
        if (path.empty())
            return;

        if (path.back() != '/')
            path.append("/");

        auto pos = path.find('/', offset);
        if (pos != std::string::npos) {
            auto dir = Tools::Read(path, pos);
            ESFileSystem::CreateFolder(dir);
            CreatePath(path, pos + 1);
        }
    }

    static void ForEachInDirWithExtRecursive(const std::string& path, const std::string& ext, const std::function<void(std::string)>& function) {
        if (!Tools::ESFileSystem::GetAllFoldersInDir) {
            ES_ERROR("EvoScript::Tools::ForEachInDirWithExtRecursive() : \"GetAllFoldersInDir\" is nullptr!");
            return;
        }

        auto&& files = GetAllFilesInDirWithExt(path, ext);
        for (auto&& file : files) {
            function(file);
        }

        auto&& folders = ESFileSystem::GetAllFoldersInDir(path);
        if (folders.empty()) {
            return;
        }

        for (auto&& folder : folders) {
            ForEachInDirWithExtRecursive(folder, ext, function);
        }
    }
}

#endif //EVOSCRIPT_FILESYSTEM_H
