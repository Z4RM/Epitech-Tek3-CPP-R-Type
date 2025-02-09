/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Sprite.hpp
*/

#ifndef RTYPE_SPRITE_HPP_
#define RTYPE_SPRITE_HPP_

#include <SFML/Audio.hpp>
#include "Components.hpp"

namespace rtype::components {
    struct Sound {
        std::string path;
        std::shared_ptr<sf::SoundBuffer> buffer;
        std::shared_ptr<sf::Sound> sound;
        bool play = false; /* Whether the sound should be played or not */
    };
}

#endif /* !RTYPE_SPRITE_HPP_ */
