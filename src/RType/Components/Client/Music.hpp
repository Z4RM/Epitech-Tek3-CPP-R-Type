/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Music.hpp
*/

#ifndef RTYPE_MUSIC_HPP_
#define RTYPE_MUSIC_HPP_

#include <memory>
#include <SFML/Audio.hpp>

namespace rtype::components {
    struct Music {
        std::string path;
        std::shared_ptr<sf::Music> music;
    };
}

#endif /* !RTYPE_MUSIC_HPP_ */
