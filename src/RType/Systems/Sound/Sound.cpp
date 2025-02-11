/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Sound.cpp
*/

#include <spdlog/spdlog.h>
#include "Components.hpp"
#include "RType/Config/Config.hpp"
#include "Sound.hpp"

void rtype::systems::Sound::play(
        rtype::ecs::EntityManager &entityManager,
        rtype::ecs::ComponentManager &componentManager
) {
    for (const auto &entity: entityManager.getEntities()) {
        auto sound = componentManager.getComponent<components::Sound>(entity);

        if (sound && sound->play) {
            sound->sound->play();
            sound->play = false;
            componentManager.addComponent(entity, *sound);
        }
    }
}

void rtype::systems::Sound::createEffect(
        const std::string &path,
        ecs::ComponentManager &componentManager,
        unsigned int entity
) {
    components::Sound projectileSound = {
            path,
            std::make_shared<sf::SoundBuffer>(),
            std::make_shared<sf::Sound>()
    };

    if (!projectileSound.buffer->loadFromFile(projectileSound.path)) {
        spdlog::error("Failed to load sound from file");
        return;
    }

    projectileSound.sound->setBuffer(*projectileSound.buffer);
    projectileSound.sound->setVolume(Config::getInstance().getSounds().volumes.effects);
    projectileSound.play = true;

    componentManager.addComponent(entity, projectileSound);
}

void rtype::systems::Sound::createMusic(
        const std::string &path,
        ecs::ComponentManager &componentManager,
        unsigned int entity,
        bool loop,
        std::optional<float> duration
) {
    components::Music menuMusic = {
            path,
            std::make_shared<sf::Music>()
    };

    if (!menuMusic.music->openFromFile(menuMusic.path)) {
        spdlog::error("Failed to load music from file");
        return;
    }

    menuMusic.music->setVolume(Config::getInstance().getSounds().volumes.music);
    if (duration.has_value())
        menuMusic.music->setLoopPoints(sf::Music::TimeSpan(sf::seconds(0), sf::seconds(duration.value())));
    if (loop)
        menuMusic.music->setLoop(true);

    // The music is not played in the system because it must be played only once and then the loop makes the rest
    menuMusic.music->play();

    // The music is added to the component manager so it's not destroyed once this function ends
    componentManager.addComponent(entity, menuMusic);
}
