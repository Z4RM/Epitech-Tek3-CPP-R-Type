/*
** EPITECH PROJECT, 2024
** R_Type
** File description:
** Player.hpp
*/

#include "Player.hpp"

#include "RType/ModeManager/ModeManager.hpp"

#ifdef RTYPE_IS_CLIENT

rtype::components::Player::Player(
        rtype::ecs::EntityManager &entityManager,
        rtype::ecs::ComponentManager &componentManager,
        const Position pos,
        const Velocity vel,
        const Size size,
        Sprite &sprite,
        const Animation &animation
) {
    _id = entityManager.createEntity();
    sprite.texture = new sf::Texture();
    sprite.sprite = new sf::Sprite();
    const int width = static_cast<int>(sprite.size.width);
    const int height = static_cast<int>(sprite.size.height);
    const sf::IntRect rect(0, 0, width, height);
    sprite.texture->loadFromFile(sprite.path, rect);
    sprite.sprite->setTexture(*sprite.texture);
    sprite.sprite->setPosition({pos.x, pos.y});
    sprite.sprite->setScale(2, 2);
    componentManager.addComponent<Sprite>(_id, sprite);
    componentManager.addComponent<Animation>(_id, animation);
    componentManager.addComponent<Position>(_id, pos);
    componentManager.addComponent<Velocity>(_id, vel);
    componentManager.addComponent<Size>(_id, size);
    componentManager.addComponent<Hitbox>(_id, {pos, size});

    // Press
    _inputs.keyActions.insert({
        sf::Keyboard::Key::Z,
        {sf::Event::KeyPressed, [this, pos, &componentManager]() {
            componentManager.getComponent<Velocity>(_id)->y = -1;
        }}
    });

    _inputs.keyActions.insert({
        sf::Keyboard::Key::S,
        {sf::Event::KeyPressed, [this, &componentManager]() {
            componentManager.getComponent<Velocity>(_id)->y = 1;
        }}
    });

    _inputs.keyActions.insert({
        sf::Keyboard::Key::Q,
        {sf::Event::KeyPressed, [this, &componentManager]() {
            componentManager.getComponent<Velocity>(_id)->x = -1;
        }}
    });

    _inputs.keyActions.insert({
        sf::Keyboard::Key::D,
        {sf::Event::KeyPressed, [this, &componentManager]() {
            componentManager.getComponent<Velocity>(_id)->x = 1;
        }}
    });

    // Release
    _inputs.keyActions.insert({
        sf::Keyboard::Key::Z,
        {sf::Event::KeyReleased, [this, pos, &componentManager]() {
            componentManager.getComponent<Velocity>(_id)->y = 0;
        }}
    });

    _inputs.keyActions.insert({
        sf::Keyboard::Key::S,
        {sf::Event::KeyReleased, [this, &componentManager]() {
            componentManager.getComponent<Velocity>(_id)->y = 0;
        }}
    });

    _inputs.keyActions.insert({
        sf::Keyboard::Key::Q,
        {sf::Event::KeyReleased, [this, &componentManager]() {
            componentManager.getComponent<Velocity>(_id)->x = 0;
        }}
    });

    _inputs.keyActions.insert({
        sf::Keyboard::Key::D,
        {sf::Event::KeyReleased, [this, &componentManager]() {
            componentManager.getComponent<Velocity>(_id)->x = 0;
        }}
    });

    _inputs.keyActions.insert({
        sf::Keyboard::Key::Space,
        {sf::Event::KeyPressed, [this, &entityManager, &componentManager]() {
            this->shoot(entityManager, componentManager);
        }}
    });

    componentManager.addComponent<InputHandler>(_id, _inputs);
}

void rtype::components::Player::shoot(
    rtype::ecs::EntityManager &entityManager,
    rtype::ecs::ComponentManager &componentManager
) const {
    if (_shootClock.getElapsedTime().asSeconds() < _shootCooldown) {
        return;
    }
    _shootClock.restart();

    size_t projectileId = entityManager.createEntity();
    Position playerPos = *componentManager.getComponent<Position>(_id);
    Velocity projectileVel = {2.0f, 0.0f, 0.0f};
    Position projectilePos = {playerPos.x + 10.0f, playerPos.y, playerPos.z};

    Sprite projectileSprite = {
        projectilePos,
        {10.0f, 10.0f},
        "assets/sprites/projectile/player-shots.gif",
        {1},
        new sf::Texture(),
        new sf::Sprite(),
        {true}
    };
    projectileSprite.texture->loadFromFile(projectileSprite.path);
    projectileSprite.sprite->setTexture(*projectileSprite.texture);

    sf::IntRect textureRect(80, 150, 85, 50);
    projectileSprite.sprite->setTextureRect(textureRect);
    projectileSprite.sprite->setPosition({projectilePos.x, projectilePos.y});

    Animation projectileAnimation = {
        "assets/sprites/projectile/player-shots.gif",
        6,
        10
    };

    componentManager.addComponent<Sprite>(projectileId, projectileSprite);
    componentManager.addComponent<Position>(projectileId, projectilePos);
    componentManager.addComponent<Velocity>(projectileId, projectileVel);
    componentManager.addComponent<Size>(projectileId, {10.0f, 10.0f});
    componentManager.addComponent<Hitbox>(projectileId, {projectilePos, {10.0f, 10.0f}});
    componentManager.addComponent<Animation>(projectileId, projectileAnimation);

    Projectile projectile = {
        projectilePos,
        projectileVel,
#ifdef RTYPE_IS_CLIENT
        projectileAnimation,
        projectileSprite
#endif
    };
    componentManager.addComponent<Projectile>(projectileId, projectile);
}
void updateProjectiles(
    rtype::ecs::ComponentManager &componentManager,
    std::vector<size_t> &projectileIds
) {
    for (size_t projectileId : projectileIds) {
        auto *projectile = componentManager.getComponent<Projectile>(projectileId);
        if (!projectile) continue;

#ifdef RTYPE_IS_CLIENT
        float elapsedTime = projectile->animationClock.getElapsedTime().asSeconds();
        int frame = static_cast<int>(elapsedTime * projectile->animation.frameRate) % projectile->animation.nbFrames;
        int frameWidth = static_cast<int>(projectile->sprite.size.width);
        sf::IntRect textureRect(frame * frameWidth, 0, frameWidth, static_cast<int>(projectile->sprite.size.height));
        projectile->sprite.sprite->setTextureRect(textureRect);
#endif
        projectile->position.x += projectile->velocity.x;
        projectile->position.y += projectile->velocity.y;
        projectile->sprite.sprite->setPosition({projectile->position.x, projectile->position.y});
    }
}


#else

rtype::components::Player::Player(
        rtype::ecs::EntityManager &entityManager,
        rtype::ecs::ComponentManager &componentManager,
        const Position pos,
        const Velocity vel,
        const Size size
) {
    _id = entityManager.createEntity();
    componentManager.addComponent<Position>(_id, pos);
    componentManager.addComponent<Velocity>(_id, vel);
    componentManager.addComponent<Size>(_id, size);
    componentManager.addComponent<Hitbox>(_id, {pos, size});
}

#endif
