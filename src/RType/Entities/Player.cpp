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
#include "RType/TextureManager/TextureManager.hpp"

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
    sprite.sprite = std::make_shared<sf::Sprite>();
    sprite.texture = TextureManager::getInstance().getTexture("player");
    sprite.sprite->setTexture(*sprite.texture);
    sf::Vector2f spriteSize(sprite.sprite->getTextureRect().width, sprite.sprite->getTextureRect().height);
    sprite.sprite->setOrigin(spriteSize.x / 2, spriteSize.y / 2);
    sprite.sprite->setPosition({pos.x, pos.y});
    sprite.sprite->setScale(2, 2);

    sf::RectangleShape hitboxRect;
    hitboxRect.setOrigin(size.width / 2, size.height / 2);
    hitboxRect.setPosition(pos.x, pos.y);
    hitboxRect.setSize({size.width, size.height});
    hitboxRect.setOutlineColor(sf::Color::Green);
    hitboxRect.setOutlineThickness(2.f);
    hitboxRect.setFillColor(sf::Color::Transparent);
    componentManager.addComponent<components::Sprite>(_id, sprite, entityManager);
    componentManager.addComponent<components::Animation>(_id, animation, entityManager);
    componentManager.addComponent<components::Position>(_id, pos, entityManager);
    componentManager.addComponent<components::Velocity>(_id, vel, entityManager);
    componentManager.addComponent<components::Size>(_id, size, entityManager);
    componentManager.addComponent<components::Hitbox>(_id, {pos, size, hitboxRect}, entityManager);
    componentManager.addComponent<components::NetId>(_id, network, entityManager);
    componentManager.addComponent<components::ActualPlayer>(_id, actualPlayer, entityManager);
    componentManager.addComponent<components::Speed>(_id, { 140 }, entityManager);
    components::Health health(1000, {pos.x - 25, pos.y - 20}, size);
    componentManager.addComponent<components::Health>(_id, health, entityManager);
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
    const auto chargedShootKeybinding = Config::getInstance().getKeybinding("charged_shoot", sf::Keyboard::Key::E);

    _inputs.keyActions.insert({
        shootKeybinding,
        {sf::Event::KeyPressed, [this, &entityManager, &componentManager, id, netId]() {
            static auto clock = std::chrono::steady_clock::now();
            bool result = this->shoot(entityManager, componentManager, id, clock, false);
            if (result) {
                network::PacketPlayerShoot sendPlayerShoot(netId, false);
                network::TCPNetwork::getInstance().sendPacket(sendPlayerShoot);
            }
        }}
    });

    _inputs.keyActions.insert({
        chargedShootKeybinding,
        {sf::Event::KeyPressed, [this, &entityManager, &componentManager, id, netId]() {
            static auto clock = std::chrono::steady_clock::now();
            auto health = componentManager.getComponent<components::Health>(id);
            if (health->value > 0) {
              bool result = this->shoot(entityManager, componentManager, id, clock, true);
              if (result) {
                  network::PacketPlayerShoot sendPlayerShoot(netId, true);
                  network::TCPNetwork::getInstance().sendPacket(sendPlayerShoot);
              }
            }
        }}
    });

    //region Press
    _inputs.keyActions.insert({
        upKeybinding,
        {sf::Event::KeyPressed, [id, pos, &componentManager, &entityManager]() {
            auto vel = componentManager.getComponent<components::Velocity>(id);
            if (vel != nullptr) {
                vel->y = -1;
                componentManager.addComponent<components::Velocity>(id, *vel, entityManager);
            }
        }}
    });

    _inputs.keyActions.insert({
        downKeybinding,
        {sf::Event::KeyPressed, [id, &componentManager, &entityManager]() {
            auto vel = componentManager.getComponent<components::Velocity>(id);
            vel->y = 1;
            componentManager.addComponent<components::Velocity>(id, *vel, entityManager);
        }}
    });

    _inputs.keyActions.insert({
        leftKeybinding,
        {sf::Event::KeyPressed, [id, &componentManager, &entityManager]() {
            auto vel = componentManager.getComponent<components::Velocity>(id);
            vel->x = -1;
            componentManager.addComponent<components::Velocity>(id, *vel, entityManager);
        }}
    });

    _inputs.keyActions.insert({
        rightKeybinding,
        {sf::Event::KeyPressed, [id, &componentManager, &entityManager]() {
            auto vel = componentManager.getComponent<components::Velocity>(id);
            vel->x = 1;
            componentManager.addComponent<components::Velocity>(id, *vel, entityManager);
        }}
    });
    //endregion

    //region Release
    _inputs.keyActions.insert({
        upKeybinding,
        {sf::Event::KeyReleased, [id, pos, &componentManager, &entityManager]() {
            auto vel = componentManager.getComponent<components::Velocity>(id);
            if (vel != nullptr) {
                vel->y = 0;
                componentManager.addComponent<components::Velocity>(id, *vel, entityManager);
            }
        }}
    });

    _inputs.keyActions.insert({
        downKeybinding,
        {sf::Event::KeyReleased, [id, &componentManager, &entityManager]() {
            auto vel = componentManager.getComponent<components::Velocity>(id);
             vel->y = 0;
             componentManager.addComponent<components::Velocity>(id, *vel, entityManager);
        }}
    });

    _inputs.keyActions.insert({
        leftKeybinding,
        {sf::Event::KeyReleased, [id, &componentManager, &entityManager]() {
            auto vel = componentManager.getComponent<components::Velocity>(id);
             vel->x = 0;
             componentManager.addComponent<components::Velocity>(id, *vel, entityManager);
        }}
    });

    _inputs.keyActions.insert({
        rightKeybinding,
        {sf::Event::KeyReleased, [id, &componentManager, &entityManager]() {
            auto vel = componentManager.getComponent<components::Velocity>(id);
             vel->x = 0;
             componentManager.addComponent<components::Velocity>(id, *vel, entityManager);
        }}
    });
    //endregion

    componentManager.addComponent<components::InputHandler>(id, _inputs, entityManager);
}

bool rtype::entities::Player::shoot(ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager, size_t id,
std::chrono::steady_clock::time_point &clock, bool isSuperProjectile) {
    const auto cooldown = isSuperProjectile ? 1.5 : 0.2;
    const auto projectileSpritePath = isSuperProjectile ? "assets/sprites/projectile/player-shots-charged.gif" : "assets/sprites/projectile/player-shots.gif";
    const int projectileDamage = isSuperProjectile ? 35 : 20;
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = now - clock;

    if (elapsed.count() < cooldown)
        return false;
    clock = std::chrono::steady_clock::now();
    size_t projectileId = entityManager.createEntity();

    auto playerPos = componentManager.getComponent<components::Position>(id);
    auto netId = componentManager.getComponent<components::NetId>(id);

    if (!playerPos)
        return false;
    const float projectileVelX = isSuperProjectile ? 0.25 : 2.0;
    components::Velocity vel = {projectileVelX, 0.0, 0.0};
    components::Position pos = {playerPos->x + 10.0f, playerPos->y, playerPos->z};

    #ifdef RTYPE_IS_CLIENT
    components::Sprite projectileSprite = {
        pos,
        {82.0f, 18.0f},
        projectileSpritePath,
        {1},
        {1.0, 1.0},
        std::make_shared<sf::Texture>(),
        std::make_shared<sf::Sprite>()
    };
    projectileSprite.texture->loadFromFile(projectileSprite.path);
    projectileSprite.sprite->setTexture(*projectileSprite.texture);
    sf::IntRect textureRect(82, 165, 82, 18);
    projectileSprite.sprite->setTextureRect(textureRect);
    projectileSprite.sprite->setOrigin(82.f / 2, 18.f / 2);
    projectileSprite.sprite->setPosition({pos.x, pos.y});
    sf::RectangleShape hitboxRect;

    hitboxRect.setOrigin(projectileSprite.size.width / 2, projectileSprite.size.height / 2);
    hitboxRect.setPosition(pos.x, pos.y);
    hitboxRect.setSize({82.0f, 18.0f});
    hitboxRect.setOutlineColor(sf::Color::Green);
    hitboxRect.setOutlineThickness(2.f);
    hitboxRect.setFillColor(sf::Color::Transparent);
    components::Animation projAnim = {
    projectileSpritePath,
        2,
        10
    };
    componentManager.addComponent<components::Animation>(projectileId, projAnim, entityManager);
    componentManager.addComponent<components::Sprite>(projectileId, projectileSprite, entityManager);
    #endif
    componentManager.addComponent<components::Position>(projectileId, pos, entityManager);
    componentManager.addComponent<components::Velocity>(projectileId, vel, entityManager);
    componentManager.addComponent<components::Size>(projectileId, {projectileSprite.size.width, projectileSprite.size.height}, entityManager);
    componentManager.addComponent<components::Hitbox>(projectileId, {pos, {10.0f, 10.0f}, hitboxRect}, entityManager);
    componentManager.addComponent<components::Speed>(projectileId, {250}, entityManager);
    componentManager.addComponent<components::Damage>(projectileId, {projectileDamage}, entityManager);
    componentManager.addComponent<components::NoDamageToPlayer>(projectileId, {true}, entityManager);
    components::Projectile projectile = {
        pos,
        vel,
#ifdef RTYPE_IS_CLIENT
        projAnim,
        projectileSprite
#endif
    };
    componentManager.addComponent<components::Projectile>(projectileId, projectile, entityManager);

    network::PacketPlayerShoot packet(netId->id, isSuperProjectile);
    return true;
}

#else

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
    componentManager.addComponent<components::Position>(_id, pos, entityManager);
    componentManager.addComponent<components::Velocity>(_id, vel, entityManager);
    componentManager.addComponent<components::Size>(_id, size, entityManager);
    componentManager.addComponent<components::Hitbox>(_id, {pos, size}, entityManager);
    componentManager.addComponent<components::NetworkConnection>(_id, network, entityManager);
    componentManager.addComponent<components::NetId>(_id, netId, entityManager);
    componentManager.addComponent<components::Speed>(_id, {140}, entityManager);

    components::Health health(1000);
    componentManager.addComponent<components::Health>(_id, health, entityManager);
}
#endif