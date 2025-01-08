/*
** EPITECH PROJECT, 2024
** RType
** File description:
** GameLoader.hpp
*/


#ifndef GAMELOADER_HPP
#define GAMELOADER_HPP

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <nlohmann/json.hpp>

class GameLoader {
public:
    nlohmann::json globalConfig;
    nlohmann::json levelConfig;

    void loadGlobalConfig(const std::string& path) {
        globalConfig = loadFile(path);
    }

    void loadLevelConfig(const std::string& path) {
        levelConfig = loadFile(path);
    }

private:
    nlohmann::json loadFile(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file: " + path);
        }
        nlohmann::json config;
        file >> config;
        return config;
    }
};


#endif //GAMELOADER_HPP
