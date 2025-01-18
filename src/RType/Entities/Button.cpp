/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Button entity implementation
*/

#include "Button.hpp"

#include "RType/Components/Client/OnClick.hpp"

namespace rtype::entities {
    Button::Button(ecs::ComponentManager &componentManager, ecs::EntityManager &entityManager, components::OnClick onClick,
    components::SfText text) :
    AEntity(entityManager) {
        componentManager.addComponent<components::OnClick>(_id, onClick);
        componentManager.addComponent<components::SfText>(_id, text);
    }
}
