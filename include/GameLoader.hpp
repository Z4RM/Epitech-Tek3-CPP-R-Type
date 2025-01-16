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

/**
 * @brief The GameLoader class is responsible for loading game configuration files.
 * This includes global configuration settings and level-specific configurations.
 */
class GameLoader {
public:
    /**
     * @brief Global configuration data loaded from a file.
     */
    nlohmann::json globalConfig;
    /**
     * @brief Level-specific configuration data loaded from a file.
     */
    nlohmann::json levelConfig;

    /**
     * @brief Loads the global configuration from a specified file path.
     *
     * @param path The path to the global configuration file.
     * @throws std::runtime_error If the file cannot be opened.
     */
    void loadGlobalConfig(const std::string& path) {
        globalConfig = loadFile(path);
    }

    /**
     * @brief Loads the level configuration from a specified file path.
     *
     * @param path The path to the level configuration file.
     * @throws std::runtime_error If the file cannot be opened.
     */
    void loadLevelConfig(const std::string& path) {
        levelConfig = loadFile(path);
    }

private:
    /**
     * @brief Loads a JSON file from the specified path.
     *
     * @param path The path to the JSON file to load.
     * @return nlohmann::json The parsed JSON object.
     * @throws std::runtime_error If the file cannot be opened.
     */
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
