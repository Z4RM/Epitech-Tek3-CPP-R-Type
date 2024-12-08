/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Structure.hpp
*/

#ifndef STRUCTURE_HPP
#define STRUCTURE_HPP

#pragma once
#include <string>

struct Position
{
    float x;
    float y;
    float z;
};

struct Size
{
    float width;
    float height;
};

struct Velocity
{
    float x;
    float y;
};

struct Hitbox
{
    Position pos;
    Size size;
};

struct Sprite
{
    Position pos;
    Size size;
    std::string path;
};

struct Animation
{
    std::string path;
    int nbFrames;
    int frameRate;
};

#endif //STRUCTURE_HPP
