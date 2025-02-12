/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Bonus Service
*/

#include "BonusService.hpp"

#include <RType/Components/Shared/Hitbox.hpp>
#include <RType/Components/Shared/Position.hpp>

#include "RType/Components/Shared/Bonus.hpp"
#include <spdlog/spdlog.h>
#include "RType/Components/Shared/EventId.hpp"

#ifdef RTYPE_IS_CLIENT
#include "RType/Components/Client/Sprite.hpp"
#include "RType/TextureManager/TextureManager.hpp"
#endif

namespace rtype::services {
    void BonusService::createBonus(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager,
    models::EBonusType type, components::Position pos, long eventId) {
        unsigned int entityId = entityManager.createEntity();

        components::Size size {};

        if (type == models::FORCE) {
            size.width = 32.0;
            size.height = 20.0;
        } else {
            size.width = 40;
            size.height = 34;
        }
        components::Bonus bonus = { type };
        components::EventId event = { eventId };
        componentManager.addComponent<components::Bonus>(entityId, bonus, entityManager);
        componentManager.addComponent<components::Position>(entityId, pos, entityManager);
        componentManager.addComponent<components::EventId>(entityId, event, entityManager);
#ifdef RTYPE_IS_SERVER
        componentManager.addComponent<components::Hitbox>(entityId, {pos, {32.0f, 20.0f}}, entityManager);
#endif
        spdlog::debug("Spawning a bonus: {} at pos: {}:{}", std::to_string(type), pos.x, pos.y);
#ifdef RTYPE_IS_CLIENT
        sf::RectangleShape hitboxRect;

        hitboxRect.setPosition(pos.x, pos.y);
        hitboxRect.setOrigin(size.width / 2, size.height / 2);
        hitboxRect.setSize({size.width, size.height});
        hitboxRect.setOutlineColor(sf::Color::Green);
        hitboxRect.setOutlineThickness(2.f);
        hitboxRect.setFillColor(sf::Color::Transparent);
        componentManager.addComponent<components::Hitbox>(entityId, {pos, {size.width, size.height}, hitboxRect}, entityManager);
        components::Sprite bonusSprite = {
            pos,
            {size.width, size.height},
            "assets/sprites/force.gif",
            {1},
            {1.0, 1.0},
            std::make_shared<sf::Texture>(),
            std::make_shared<sf::Sprite>()
        };
        if (type == models::FORCE)
            bonusSprite.texture = TextureManager::getInstance().getTexture("force");
        else
            bonusSprite.texture = TextureManager::getInstance().getTexture("shield");
        bonusSprite.sprite->setTexture(*bonusSprite.texture, false);
        sf::Vector2f spriteSize(size.width, size.height);
        bonusSprite.sprite->setOrigin(spriteSize.x / 2, spriteSize.y / 2);
        bonusSprite.sprite->setPosition({pos.x, pos.y});
        componentManager.addComponent<components::Sprite>(entityId, bonusSprite, entityManager);

#endif
    }
}
