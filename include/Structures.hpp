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
#include <unordered_map>
#ifdef RTYPE_IS_CLIENT
#include <SFML/Graphics.hpp>
#endif

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

struct IA {
    std::unordered_map<float, Velocity> moves;
};

#ifdef RTYPE_IS_CLIENT
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
#endif

struct Projectile
{
    Position position{};
    Velocity velocity{};
#ifdef RTYPE_IS_CLIENT
    Animation animation;
    Sprite sprite;
    sf::Clock animationClock;
#endif
};


#endif //STRUCTURE_HPP
