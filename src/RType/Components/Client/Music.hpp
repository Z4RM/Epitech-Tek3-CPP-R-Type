/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Music.hpp
*/


#pragma once
#include <SFML/Audio.hpp>
#include "IComponent.hpp"

namespace rtype::components {
    struct Music : public IComponent
    {
        void create(nlohmann::basic_json<> &value) override {
            if (!music->openFromFile(value["path"])) {
                std::cerr << "Erreur lors du chargement de la musique depuis " << value["path"] << std::endl;
                return;
            }

            music->setLoop(value["loop"]);
            music->setVolume(value["volume"]);
            music->play();
        }

        sf::Music *music = new sf::Music();
    };
}
