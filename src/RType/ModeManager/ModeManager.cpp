/*
** EPITECH PROJECT, 2024
** RType
** File description:
** ModeManager.cpp
*/

#include "spdlog/spdlog.h"
#include "ModeManager.hpp"

#ifdef RTYPE_IS_SERVER

bool rtype::ModeManager::isServer() {
    return true;
}

#else

void rtype::ModeManager::enableServer() {
    std::lock_guard<std::mutex> lock(_modeMutex);
    _isServer = true;
    spdlog::info("Server enabled");
}

void rtype::ModeManager::disableServer() {
    std::lock_guard<std::mutex> lock(_modeMutex);
    _isServer = false;
    spdlog::info("Server disabled");
}

void rtype::ModeManager::switchServer() {
    if (_isServer)
        disableServer();
    else
        enableServer();
}

bool rtype::ModeManager::isServer() {
    return _isServer;
}

bool rtype::ModeManager::_isServer = false;

std::mutex rtype::ModeManager::_modeMutex;

#endif
