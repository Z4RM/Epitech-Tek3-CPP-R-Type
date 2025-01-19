/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main.cpp
*/

#include "RType/RType.hpp"
#include <exception>

/**
 * @see rtype::RType::run
 */
int main() {
    try {
        return  rtype::RType::run();
    } catch (std::exception &e) {
        return 84;
    }
}
