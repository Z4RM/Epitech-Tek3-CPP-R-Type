/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Enemy.cpp
*/


#include "Enemy.hpp"

#ifdef RTYPE_IS_CLIENT

rtype::entities::Enemy::Enemy(
    rtype::ecs::EntityManager &entityManager,
    rtype::ecs::ComponentManager &componentManager,
    components::Position pos,
    components::Velocity vel,
    components::Size size,
    components::Sprite &sprite,
    const components::Animation &animation,
    components::NetId netId,
    components::Speed speed
) {
    _id = entityManager.createEntity();
    sprite.texture = std::make_shared<sf::Texture>();
    sprite.sprite = std::make_shared<sf::Sprite>();
    const int width = static_cast<int>(sprite.size.width);
    const int height = static_cast<int>(sprite.size.height);
    const sf::IntRect rect(0, 0, width, height);
    sprite.texture->loadFromFile(sprite.path, rect);
    sprite.sprite->setTexture(*sprite.texture);
    sprite.sprite->setPosition({pos.x, pos.y});
    sprite.sprite->setScale(2, 2);
    componentManager.addComponent<components::Sprite>(_id, sprite);
    componentManager.addComponent<components::Animation>(_id, animation);
    componentManager.addComponent<components::Position>(_id, pos);
    componentManager.addComponent<components::Velocity>(_id, vel);
    componentManager.addComponent<components::Size>(_id, size);
    componentManager.addComponent<components::Hitbox>(_id, {pos, size});
    componentManager.addComponent<components::Speed>(_id, speed);
    componentManager.addComponent<components::NetId>(_id, netId);

    components::Health health(50, {sprite.pos.x, sprite.pos.y}, size);
    componentManager.addComponent<components::Health>(_id, health);

    components::Damage damage = {200};
    componentManager.addComponent<components::Damage>(_id, damage);


    std::unordered_map<float, components::Velocity> move;
    move.insert({
        0,
        {-0.5, 0, 0}
    });
    componentManager.addComponent<components::IA>(_id, {move});
}

void rtype::entities::Enemy::enemy_shots(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager) {
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = now - _elapsedShoot;

    if (elapsed.count() >= _shootCooldown) {
        size_t projectileId = entityManager.createEntity();
        auto enemyPos = componentManager.getComponent<components::Position>(_id);

        if (enemyPos) {
            components::Velocity vel = {-2.0, 0.0, 0.0};
            components::Position pos = {enemyPos->x - 10.0f, enemyPos->y, enemyPos->z};

            components::Sprite projectileSprite = {
                pos,
                {82.0f, 18.0f},
                "assets/sprites/projectile/player-shots.gif",
                {1},
                {1.0, 1.0},
                std::make_shared<sf::Texture>(),
                std::make_shared<sf::Sprite>(),
                165,
                82
            };
            projectileSprite.texture->loadFromFile(projectileSprite.path);
            projectileSprite.sprite->setTexture(*projectileSprite.texture);
            projectileSprite.sprite->setPosition({pos.x, pos.y});
            componentManager.addComponent<components::Sprite>(projectileId, projectileSprite);

            components::Animation projAnim = {
                "assets/sprite/projectile/player-shots.gif",
                2,
                10
            };

            componentManager.addComponent<components::Animation>(projectileId, projAnim);
            componentManager.addComponent<components::Position>(projectileId, pos);
            componentManager.addComponent<components::Velocity>(projectileId, vel);
            componentManager.addComponent<components::Size>(projectileId, {10, 10});

            _elapsedShoot = std::chrono::steady_clock::now();
        }
    }
}

#else

rtype::entities::Enemy::Enemy(
    rtype::ecs::EntityManager &entityManager,
    rtype::ecs::ComponentManager &componentManager,
    components::Position pos,
    components::Velocity vel,
    components::Size size,
    components::NetId netId,
    components::Speed speed
) {
    _id = entityManager.createEntity();
    componentManager.addComponent<components::Position>(_id, pos);
    componentManager.addComponent<components::Velocity>(_id, vel);
    componentManager.addComponent<components::Size>(_id, size);
    componentManager.addComponent<components::Hitbox>(_id, {pos, size});
    componentManager.addComponent<components::Speed>(_id, speed);
    componentManager.addComponent<components::NetId>(_id, netId);

    components::Health health = { 50, 50};
    componentManager.addComponent<components::Health>(_id, health);

    components::Damage damage = {200};
    componentManager.addComponent<components::Damage>(_id, damage);

    std::unordered_map<float, components::Velocity> move;
    move.insert({
        0,
        {-0.5, 0, 0}
    });
    componentManager.addComponent<components::IA>(_id, {move});
}

#endif
