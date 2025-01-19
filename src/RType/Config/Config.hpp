/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Config.hpp
*/

#ifndef RTYPE_CONFIG_HPP_
#define RTYPE_CONFIG_HPP_

#include "INIReader.h"
#include "spdlog/spdlog.h"

namespace rtype {
    /**
     * @class Config
     *
     * @brief Initialize all that is needed from the configuration file (e.g. log level).
     * For configuration keys that it can't do anything with, it stores them, and they are accessible from getters.
     * This class is a singleton, which means that it can be accessed from anywhere without passing it through all functions.
     */
    class Config {
        /**
         * @typedef An (unordered) map of spdlog log levels.
         */
        using LogLevels = std::unordered_map<std::string, spdlog::level::level_enum>;

        /**
         * @brief Network configuration keys.
         *
         * @see `config.ini.example`
         */
        struct Network {
            /**
             * @brief The server configuration.
             */
            struct {
#ifdef RTYPE_IS_CLIENT
                /**
                 * @brief The address of the server.
                 */
                std::string address;
#endif
                /**
                 * @brief The port of the server.
                 */
                unsigned short port;
            } server;
        };

    public:
        /**
         * @brief Get the singleton instance of Config.
         *
         * @param filename Optional parameter to specify the configuration file from which the configuration should be read.
         * It will be used only during the first call.
         *
         * @return Reference to the Config singleton instance.
         */
        static Config &getInstance(const std::string &filename = "./config.ini") {
            static Config instance(filename);

            return instance;
        }

        /**
         * @brief Alias for `getInstance` to simply initialize the class,
         * without returning an instance of it, but the success status of the initialization.
         *
         * @param filename Optional parameter to specify the configuration file from which the configuration should be read.
         *
         * @return `true` if all was initialized successfully, `false` otherwise.
         */
        static bool initialize(const std::string &filename = "config.ini") {
            getInstance(filename);
            return _valid;
        }

        /**
         * @return The network configuration.
         */
        [[nodiscard]] Network getNetwork() const;

        //region Delete copy and move constructors to ensure singleton integrity
        Config(const Config &) = delete;

        Config &operator=(const Config &) = delete;

        Config(Config &&) = delete;

        Config &operator=(Config &&) = delete;
        //endregion

    private:
        /**
         * @brief Private constructor.
         * It reads and parses the config and initialize all that is needed.
         *
         * @param filename The configuration file from which the configuration should be read.
         */
        explicit Config(const std::string &filename);

        /**
         * @brief Set the spdlog log level, depending on the log level defined in the configuration file (if applicable).
         *
         * @param reader The INIReader where to get the configuration value from.
         */
        static void _setLogLevel(const INIReader &reader);

        /**
         * @brief Get, validate and store the network configuration values.
         *
         * @param reader The INIReader where to get the configuration value from.
         */
        void _initializeNetwork(const INIReader &reader);

        /**
         * @var Map containing the log levels.
         * It permits to get the spdlog level value from its name (as string).
         */
        static const LogLevels _logLevels;

        /**
         * @var Whether the initialization is successful or not.
         * Used (returned) by `initialize`.
         */
        static bool _valid;

        /**
         * @var The network configuration.
         *
         * @see Network
         */
        Network _network;
    };
}

#endif /* !RTYPE_CONFIG_HPP_ */
