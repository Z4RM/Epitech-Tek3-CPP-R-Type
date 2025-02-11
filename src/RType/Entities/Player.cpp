/*
** EPITECH PROJECT, 2024
** R_Type
** File description:
** Player.hpp
*/

#include "Systems.hpp"
#include "RType/ModeManager/ModeManager.hpp"
#include "Player.hpp"

#include "RType/Components/Shared/EventId.hpp"
#include "RType/Config/Config.hpp"
#include "RType/Services/ProjectileService/ProjectileService.hpp"
#include "RType/Config/Config.hpp"
#include "Player.hpp"


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
    std::string textureName = "player" + std::to_string(network.id);
    if (network.id > 1 && network.id < 5)
        sprite.texture = TextureManager::getInstance().getTexture(textureName);
    else
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
            auto health = componentManager.getComponent<components::Health>(id);
            if (health->value > 0) {
             bool result = this->shoot(entityManager, componentManager, id, clock, false);
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

bool rtype::entities::Player::shoot(
        ecs::EntityManager &entityManager,
        ecs::ComponentManager &componentManager,
        size_t id,
        std::chrono::steady_clock::time_point &clock,
        bool isSuperProjectile
) {
    const auto cooldown = isSuperProjectile ? 1.5 : 0.2;
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = now - clock;

    if (elapsed.count() < cooldown)
        return false;
    clock = std::chrono::steady_clock::now();

    auto playerPos = componentManager.getComponent<components::Position>(id);
    auto netId = componentManager.getComponent<components::NetId>(id);

    if (!playerPos)
        return false;
    components::globalEventId.store(components::globalEventId.load() + 1);
    components::EventId event = {components::globalEventId.load(), netId->id};
    services::ProjectileService::createProjectile(entityManager, componentManager, playerPos, isSuperProjectile, event);
    //systems::Sound::createEffect("assets/sounds/effects/shoot.wav", componentManager, projectileId);
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