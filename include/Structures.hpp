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
#include <SFML/Graphics.hpp>

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
    float z;
};

struct Hitbox
{
    Position pos;
    Size size;
};

struct Created
{
    bool isCreate = false;
};

struct ZIndex
{
    int value;
};

struct Sprite
{
    Position pos;
    Size size;
    std::string path;
    ZIndex priority;
    sf::Texture* texture;
    sf::Sprite* sprite;
    Created created;
};

struct Animation
{
    std::string path;
    int nbFrames;
    int frameRate;
};

struct String
{
    std::string s;
};

#endif //STRUCTURE_HPP
