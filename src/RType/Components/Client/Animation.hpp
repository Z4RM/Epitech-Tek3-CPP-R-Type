/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Animation.hpp
*/

#pragma once

#include <string>

namespace rtype::components {
    struct Animation
    {
        std::string path;
        int nbFrames;
        int frameRate;
    };
}