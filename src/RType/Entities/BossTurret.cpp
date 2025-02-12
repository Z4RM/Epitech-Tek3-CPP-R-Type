/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Boss turret
*/

#include "BossTurret.hpp"

#include <spdlog/spdlog.h>

#include "Components.hpp"
#include "RType/Components/Server/AiShoot.hpp"
#include "RType/Components/Shared/ChildEntities.hpp"
#include "RType/Components/Shared/ParentEntity.hpp"
#include "RType/Services/EnemyService/EnemyService.hpp"
#include "RType/Systems/Network/Network.hpp"

#ifdef RTYPE_IS_CLIENT
#include "RType/TextureManager/TextureManager.hpp"
namespace rtype::entities {
    BossTurret::BossTurret(ecs::ComponentManager &componentManager, ecs::EntityManager &entityManager, components::Position
    pos, components::NetId netId) : AEntity(entityManager) {
        _id = entityManager.createEntity();
        components::Sprite sprite;
        sprite.priority = { 1 };
        components::Size size = {592 / 2, 176 / 2};

        sprite.sprite = std::make_shared<sf::Sprite>();
        sprite.texture = TextureManager::getInstance().getTexture("boss_turret");
        sprite.sprite->setTexture(*sprite.texture);
        sf::Vector2f spriteSize(sprite.sprite->getTextureRect().width, sprite.sprite->getTextureRect().height);
        sprite.sprite->setScale(0.5, 0.5);
        sprite.sprite->setOrigin(spriteSize.x / 2, spriteSize.y / 2);
        sprite.sprite->setPosition({pos.x, pos.y});

        sf::RectangleShape hitboxRect;

        hitboxRect.setOrigin(size.width / 2, size.height / 2);
        hitboxRect.setPosition(pos.x, pos.y);
        hitboxRect.setSize({size.width, size.height});
        hitboxRect.setOutlineColor(sf::Color::Green);
        hitboxRect.setOutlineThickness(2.f);
        hitboxRect.setFillColor(sf::Color::Transparent);
        componentManager.addComponent<components::Position>(_id, pos, entityManager);
        componentManager.addComponent<components::Size>(_id, size, entityManager);
        componentManager.addComponent<components::Hitbox>(_id, {pos, size, hitboxRect}, entityManager);
        componentManager.addComponent<components::NetId>(_id, netId, entityManager);
        componentManager.addComponent<components::Velocity>(_id, {0, 0}, entityManager);
        componentManager.addComponent<components::Size>(_id, {592, 176}, entityManager);
        componentManager.addComponent<components::Speed>(_id, {1}, entityManager);

        components::Health health(2000, {sprite.pos.x, sprite.pos.y}, size);
        componentManager.addComponent<components::Health>(_id, health, entityManager);

        components::Damage damage = {200};
        componentManager.addComponent<components::Damage>(_id, damage, entityManager);


        std::vector<components::Velocity> move;
        move.emplace_back(components::Velocity({0, 1, 0}));
        move.emplace_back(components::Velocity({0, -1, 0}));
        componentManager.addComponent<components::IA>(_id, {move, models::BOSS_TURRET, 1, 2}, entityManager);
        componentManager.addComponent<components::Sprite>(_id, sprite, entityManager);
    }
}
#else
namespace rtype::entities {
    BossTurret::BossTurret(ecs::ComponentManager &componentManager, ecs::EntityManager &entityManager, components::Position
    pos, components::NetId netId)
     : AEntity
    (entityManager) {
        componentManager.addComponent<components::Position>(_id, pos, entityManager);
        componentManager.addComponent<components::Hitbox>(_id, {pos, 592 / 2, 176 / 2}, entityManager);
        componentManager.addComponent<components::NetId>(_id, netId, entityManager);
        componentManager.addComponent<components::Velocity>(_id, {0, 0}, entityManager);
        componentManager.addComponent<components::Size>(_id, {592 / 2, 176 / 2}, entityManager);
        componentManager.addComponent<components::Speed>(_id, {200 }, entityManager);

        components::Health health(2000);
        componentManager.addComponent<components::Health>(_id, health, entityManager);

        components::Damage damage = {200};
        componentManager.addComponent<components::Damage>(_id, damage, entityManager);

        std::vector<components::Velocity> move;
        move.emplace_back(components::Velocity({0, 1, 0}));
        move.emplace_back(components::Velocity({0, -1, 0}));
        componentManager.addComponent<components::IA>(_id, {move, models::BOSS_TURRET, 1, 2}, entityManager);
        componentManager.addComponent<components::ChildEntities>(_id,  {}, entityManager);
        BossTurret::createTurret(_id, componentManager, entityManager, {pos.x - 110, pos.y - 20});
        BossTurret::createTurret(_id, componentManager, entityManager, {pos.x - 170, pos.y});
        BossTurret::createTurret(_id, componentManager, entityManager, {pos.x - 110, pos.y + 40});
    }

     void BossTurret::createTurret(unsigned int parentEntity, ecs::ComponentManager &componentManager,
    ecs::EntityManager &entityManager, components::Position turretPos, float cooldown) {
        systems::Network::globalNetId.store(systems::Network::globalNetId.load() + 1);
        auto parentPos = componentManager.getComponent<components::Position>(parentEntity);

        unsigned int turretEntity = services::EnemyService::createTurret(entityManager, componentManager, turretPos, systems::Network::globalNetId.load());
        componentManager.addComponent<components::ParentEntity>(turretEntity, { parentEntity, turretPos.x - parentPos->x, turretPos.y - parentPos->y }, entityManager);

        auto childEntities = componentManager.getComponent<components::ChildEntities>(parentEntity);
        if (childEntities) {
            childEntities->childEntities.emplace_back(turretEntity);
            componentManager.addComponent<components::ChildEntities>(parentEntity, *childEntities, entityManager);
        }
        componentManager.addComponent<components::AiShoot>(turretEntity, { cooldown }, entityManager);
    }
}
#endif