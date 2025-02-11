/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Config.cpp
*/

#ifdef RTYPE_IS_CLIENT
#include "Keys.hpp"
#endif
#include "Config.hpp"

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

rtype::Config::Config(const std::string &filename) : _reader(filename) {
    _setLogLevel();
    _initializeNetwork();
#ifdef RTYPE_IS_CLIENT
    _initializeSounds();
#endif
}

void rtype::Config::_setLogLevel() {
    auto logLevel = _reader.GetString("log", "level", "");

    if (logLevel == "debug") {
        this->isLogLevelDebug = true;
    }
    if (!logLevel.empty()) {
        if (_logLevels.find(logLevel) == _logLevels.end())
            spdlog::warn("\"\33[3m" + logLevel + "\33[0m\" is not a valid log level, ignoring this setting");
        else
            spdlog::set_level(_logLevels.at(logLevel));
    }
}

void rtype::Config::_initializeNetwork() {
#ifdef RTYPE_IS_CLIENT
    _network.server.address = _reader.GetString("network", "server_address", "127.0.0.1");
    if (_network.server.address.empty())
        spdlog::warn("No server address provided, you will be able to play only with a \"local server\"");
#endif

    auto port = _reader.GetUnsigned("network", "server_port", 0);
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

#ifdef RTYPE_IS_CLIENT
void rtype::Config::_initializeSounds() {
    try {
        _sounds.volumes.effects = std::stof(_reader.GetString("sounds", "effects_volume", "50"));
        _sounds.volumes.music = std::stof(_reader.GetString("sounds", "music_volume", "10"));
    } catch (const std::invalid_argument &exception) {
        spdlog::warn("Invalid volume value provided");
    }
    spdlog::info("{} {}", _sounds.volumes.effects, _sounds.volumes.music);
}

rtype::Config::Sounds rtype::Config::getSounds() const {
    return _sounds;
}
#endif

#ifdef RTYPE_IS_CLIENT
sf::Keyboard::Key rtype::Config::getKeybinding(const std::string &key, sf::Keyboard::Key fallback) const {
    const auto keybinding = _reader.GetString("keybindings", key, "");

    if (keybinding.empty()) {
        spdlog::warn("Keybinding for \"\33[3m" + key + "\33[0m\" not found, using fallback keybinding");
        return fallback;
    }

    if (keys.find(keybinding) == keys.end()) {
        spdlog::warn("Keybinding for \"\33[3m" + key + "\33[0m\" (\"\33[3m" + keybinding + "\33[0m\") is not a valid key, using fallback keybinding");
        return fallback;
    }

    return keys.at(keybinding);
}

#endif
