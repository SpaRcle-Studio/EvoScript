//
// Created by Nikita on 08.07.2021.
//

#ifndef EVOSCRIPT_FILESYSTEM_H
#define EVOSCRIPT_FILESYSTEM_H

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include <EvoScript/Tools/StringUtils.h>

#include <experimental/filesystem> // or #include <filesystem> for C++17 and up
#include <fstream>
#include <vector>

namespace fs = std::experimental::filesystem;

namespace EvoScript::Tools {
    static std::vector<std::string> GetAllFilesInDirWithExt(const std::string& path, const std::string& ext) {
        auto files = std::vector<std::string>();
        for (const auto & entry : fs::directory_iterator(path))
            if (entry.path().extension() == ext)
                files.emplace_back(entry.path().string());
        return files;
    }

    static bool FileExists(const std::string& path) {
        struct stat buffer = {};
        return (stat(path.c_str(), &buffer) == 0);
    }

    static bool RemoveFile(const std::string& path) {
        return std::remove(path.c_str());
    }

    static bool CreateDirectory(const std::string& directory) {
        return fs::create_directory(directory);
    }

    static bool Copy(const std::string& src, const std::string& dst) {
        if (!FileExists(src))
             return false;
        else {
            std::ifstream src_f(src, std::ios::binary);
            std::ofstream dst_f(dst, std::ios::binary);

            dst_f << src_f.rdbuf();

            src_f.close();
            dst_f.close();

            return true;
        }
    }

    //! path must be a fix
    static void CreatePath(const std::string& path, uint32_t offset = 0) {
        auto pos = path.find('/', offset);
        if (pos != std::string::npos) {
            auto dir = Tools::Read(path, pos);
            Tools::CreateDirectory(dir);
            CreatePath(path, pos + 1);
        }
    }
}

#endif //EVOSCRIPT_FILESYSTEM_H
