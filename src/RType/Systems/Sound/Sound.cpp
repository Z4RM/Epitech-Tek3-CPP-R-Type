/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Sound.cpp
*/

#include "Components.hpp"
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
