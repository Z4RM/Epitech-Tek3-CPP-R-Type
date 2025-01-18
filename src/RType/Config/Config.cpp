/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Config.cpp
*/

#include "Config.hpp"

#include <iostream>

const rtype::Config::LogLevels rtype::Config::_logLevels = {
        {"trace",    spdlog::level::trace},
        {"debug",    spdlog::level::debug},
        {"info",     spdlog::level::info},
        {"warn",     spdlog::level::warn},
        {"err",      spdlog::level::err},
        {"critical", spdlog::level::critical},
        {"off",      spdlog::level::off}
};

bool rtype::Config::_valid = true;

rtype::Config::Config(const std::string &filename) {
    INIReader reader(filename);

    _setLogLevel(reader);
    _initializeNetwork(reader);
}

void rtype::Config::_setLogLevel(const INIReader &reader) {
    auto logLevel = reader.GetString("log", "level", "");

    if (!logLevel.empty()) {
        if (_logLevels.find(logLevel) == _logLevels.end())
            spdlog::warn("\"\33[3m" + logLevel + "\33[0m\" is not a valid log level");
        else
            spdlog::set_level(_logLevels.at(logLevel));
    }
}

void rtype::Config::_initializeNetwork(const INIReader &reader) {
#ifdef RTYPE_IS_CLIENT
    _network.server.address = reader.GetString("network", "server_address", "");
    std::cout << _network.server.address <<std::endl;
    if (_network.server.address.empty())
        spdlog::warn("No server address provided, you will be able to play only with a \"local server\"");
#endif

    auto port = reader.GetUnsigned("network", "server_port", 0);
    if (port >= 1024 && port <= 65535) {
        _network.server.port = port;
    } else {
        _valid = false;
        spdlog::critical("Port must be provided and in the range [1024, 65535]");
    }
}

rtype::Config::Network rtype::Config::getNetwork() const {
    return _network;
}
