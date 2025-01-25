/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main.cpp
*/

#include "RType/RType.hpp"
#include <exception>
#include <spdlog/spdlog.h>

/**
 * @see rtype::RType::run
 */
int main() {
    // TODO: remove this try/catch and catch at the right place(s)
    try {
        return  rtype::RType::run();
    } catch (std::exception &e) {
        spdlog::critical(e.what());
        return 84;
    }
}
