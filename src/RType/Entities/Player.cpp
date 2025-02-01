/*
** EPITECH PROJECT, 2024
** R_Type
** File description:
** Player.hpp
*/

#include "RType/ModeManager/ModeManager.hpp"
#include "Player.hpp"
#include "RType/Config/Config.hpp"

#ifdef RTYPE_IS_CLIENT

rtype::entities::Player::Player(
        rtype::ecs::EntityManager &entityManager,
        rtype::ecs::ComponentManager &componentManager,
        const components::Position pos,
        const components::Velocity vel,
        const components::Size size,
        components::Sprite &sprite,
        const components::Animation &animation,
        const components::NetId network,
        components::ActualPlayer actualPlayer,
        const components::Speed speed
) {
    this->_id = entityManager.createEntity();
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
    componentManager.addComponent<components::NetId>(_id, network);
    componentManager.addComponent<components::ActualPlayer>(_id, actualPlayer);
    componentManager.addComponent<components::Speed>(_id, { 140 });
    components::Health health(1000, {pos.x, pos.y}, size);
    componentManager.addComponent<components::Health>(_id, health);
    _elaspedShoot = std::chrono::steady_clock::now();
    this->_shootCooldown = 0.8;

    if (!actualPlayer.value)
        return;

    size_t id = this->_id;
    int netId = network.id;

    const auto upKeybinding = Config::getInstance().getKeybinding("up", sf::Keyboard::Key::Z);
    const auto downKeybinding = Config::getInstance().getKeybinding("down", sf::Keyboard::Key::S);
    const auto leftKeybinding = Config::getInstance().getKeybinding("left", sf::Keyboard::Key::Q);
    const auto rightKeybinding = Config::getInstance().getKeybinding("right", sf::Keyboard::Key::D);
    const auto shootKeybinding = Config::getInstance().getKeybinding("shoot", sf::Keyboard::Key::Space);

    _inputs.keyActions.insert({
        shootKeybinding,
        {sf::Event::KeyPressed, [this, &entityManager, &componentManager, id, netId]() {
            static auto clock = std::chrono::steady_clock::now();
            bool result = this->shoot(entityManager, componentManager, id, clock);
            if (result) {
                network::PacketPlayerShoot sendPlayerShoot(netId);
                network::TCPNetwork::getInstance().sendPacket(sendPlayerShoot);
            }
        }}
    });

    //region Press
    _inputs.keyActions.insert({
        upKeybinding,
        {sf::Event::KeyPressed, [id, pos, &componentManager]() {
            auto vel = componentManager.getComponent<components::Velocity>(id);
            if (vel != nullptr) {
                vel->y = -1;
                componentManager.addComponent<components::Velocity>(id, *vel);
            }
        }}
    });

    _inputs.keyActions.insert({
        downKeybinding,
        {sf::Event::KeyPressed, [id, &componentManager]() {
            auto vel = componentManager.getComponent<components::Velocity>(id);
            vel->y = 1;
            componentManager.addComponent<components::Velocity>(id, *vel);
        }}
    });

    _inputs.keyActions.insert({
        leftKeybinding,
        {sf::Event::KeyPressed, [id, &componentManager]() {
            auto vel = componentManager.getComponent<components::Velocity>(id);
            vel->x = -1;
            componentManager.addComponent<components::Velocity>(id, *vel);
        }}
    });

    _inputs.keyActions.insert({
        rightKeybinding,
        {sf::Event::KeyPressed, [id, &componentManager]() {
            auto vel = componentManager.getComponent<components::Velocity>(id);
            vel->x = 1;
            componentManager.addComponent<components::Velocity>(id, *vel);
        }}
    });
    //endregion

    //region Release
    _inputs.keyActions.insert({
        upKeybinding,
        {sf::Event::KeyReleased, [id, pos, &componentManager]() {
            auto vel = componentManager.getComponent<components::Velocity>(id);
            if (vel != nullptr) {
                vel->y = 0;
                componentManager.addComponent<components::Velocity>(id, *vel);
            }
        }}
    });

    _inputs.keyActions.insert({
        downKeybinding,
        {sf::Event::KeyReleased, [id, &componentManager]() {
            auto vel = componentManager.getComponent<components::Velocity>(id);
             vel->y = 0;
             componentManager.addComponent<components::Velocity>(id, *vel);
        }}
    });

    _inputs.keyActions.insert({
        leftKeybinding,
        {sf::Event::KeyReleased, [id, &componentManager]() {
            auto vel = componentManager.getComponent<components::Velocity>(id);
             vel->x = 0;
             componentManager.addComponent<components::Velocity>(id, *vel);
        }}
    });

    _inputs.keyActions.insert({
        rightKeybinding,
        {sf::Event::KeyReleased, [id, &componentManager]() {
            auto vel = componentManager.getComponent<components::Velocity>(id);
             vel->x = 0;
             componentManager.addComponent<components::Velocity>(id, *vel);
        }}
    });
    //endregion

    componentManager.addComponent<components::InputHandler>(id, _inputs);
}

bool rtype::entities::Player::shoot(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager, size_t id,
std::chrono::steady_clock::time_point &clock) {
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = now - clock;

    if (elapsed.count() < 0.2) {
        return false;
    }
    clock = std::chrono::steady_clock::now();
    size_t projectileId = entityManager.createEntity();

    auto playerPos = componentManager.getComponent<components::Position>(id);
    auto netId = componentManager.getComponent<components::NetId>(id);

    if (!playerPos) {
        return false;
    }
    components::Velocity vel = {2.0, 0.0, 0.0};
    components::Position pos = {playerPos->x + 10.0f, playerPos->y, playerPos->z};

    #ifdef RTYPE_IS_CLIENT
    components::Sprite projectileSprite = {
        pos,
        {82.0f, 18.0f},
        "assets/sprites/projectile/player-shots.gif",
        {1},
        {1.0, 1.0},
        std::make_shared<sf::Texture>(),
        std::make_shared<sf::Sprite>()
    };
    projectileSprite.texture->loadFromFile(projectileSprite.path);
    projectileSprite.sprite->setTexture(*projectileSprite.texture);
    sf::IntRect textureRect(82, 165, 82, 18);
    projectileSprite.sprite->setTextureRect(textureRect);
    projectileSprite.sprite->setPosition({pos.x, pos.y});
    components::Animation projAnim = {
    "assets/sprite/projectile/player-shots.gif",
        2,
        10
    };
    componentManager.addComponent<components::Animation>(projectileId, projAnim);
    componentManager.addComponent<components::Sprite>(projectileId, projectileSprite);
    #endif
    componentManager.addComponent<components::Position>(projectileId, pos);
    componentManager.addComponent<components::Velocity>(projectileId, vel);
    componentManager.addComponent<components::Size>(projectileId, {10.0f, 10.0f});
    componentManager.addComponent<components::Hitbox>(projectileId, {pos, {10.0f, 10.0f}});
    componentManager.addComponent<components::Speed>(projectileId, {250});
    componentManager.addComponent<components::Damage>(projectileId, {20});
    componentManager.addComponent<components::NoDamageToPlayer>(projectileId, {true});
    components::Projectile projectile = {
        pos,
        vel,
#ifdef RTYPE_IS_CLIENT
        projAnim,
        projectileSprite
#endif
    };
    componentManager.addComponent<components::Projectile>(projectileId, projectile);

    network::PacketPlayerShoot packet(netId->id);
    return true;
}

#endif

rtype::entities::Player::Player(
        rtype::ecs::EntityManager &entityManager,
        rtype::ecs::ComponentManager &componentManager,
        const components::Position pos,
        const components::Velocity vel,
        const components::Size size,
        const components::NetworkConnection &network,
        const components::NetId netId,
        const components::Speed speed
) {
    _id = entityManager.createEntity();
    componentManager.addComponent<components::Position>(_id, pos);
    componentManager.addComponent<components::Velocity>(_id, vel);
    componentManager.addComponent<components::Size>(_id, size);
    componentManager.addComponent<components::Hitbox>(_id, {pos, size});
    componentManager.addComponent<components::NetworkConnection>(_id, network);
    componentManager.addComponent<components::NetId>(_id, netId);
    componentManager.addComponent<components::Speed>(_id, {140});

    components::Health health(1000);
    componentManager.addComponent<components::Health>(_id, health);
}
