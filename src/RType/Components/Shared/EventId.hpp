/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Event id
*/

#pragma once

#include <atomic>

namespace rtype::components {
    static std::atomic<long> globalEventId = 0;
    struct EventId {
        long value = 0;
        int netIdEmitter = -1;
    };
}