cmake_minimum_required(VERSION 3.28)

project(RType VERSION 1.0.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
foreach (OUTPUT_CONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUT_CONFIG} OUTPUT_CONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUT_CONFIG} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endforeach ()

find_package(inih REQUIRED)
find_package(spdlog REQUIRED)
find_package(asio REQUIRED)
find_package(SFML 2.6 COMPONENTS system window graphics network audio REQUIRED)
find_package(nlohmann_json REQUIRED)

include_directories(include src)

set(RTYPE_SRC
        src/main.cpp
        src/ECS/ComponentManager.hpp
        src/ECS/EntityManager.hpp
        src/ECS/SparseSet.hpp
        src/ECS/SystemManager.hpp
        src/RType/Entities/Enemy.cpp
        src/RType/Entities/Enemy.hpp
        src/RType/Entities/Player.cpp
        src/RType/Entities/Player.hpp
        src/RType/RType.cpp
        src/RType/RType.hpp
        src/RType/Config/Config.cpp
        src/RType/Config/Config.hpp
        src/RType/ModeManager/ModeManager.cpp
        src/RType/ModeManager/ModeManager.hpp
        src/RType/Systems/Movement.cpp
        src/RType/Systems/Movement.hpp
        src/Network/UDPNetwork/UDPNetwork.cpp
        src/Network/UDPNetwork/UDPNetwork.hpp
        src/Network/TCPNetwork/TCPNetwork.cpp
        src/Network/TCPNetwork/TCPNetwork.hpp
        src/RType/Systems/Network/Network.cpp
        src/RType/Systems/Network/Network.hpp
        src/ThreadPool/ThreadPool.cpp
        src/Network/Packets/PacketFactory/PacketFactory.cpp
        src/Network/Packets/PacketFactory/PacketFactory.hpp
        src/Network/Packets/Descriptors/PacketConnect/PacketConnect.cpp
        src/Network/Packets/Descriptors/PacketWelcome/PacketWelcome.cpp
        src/Network/Packets/Descriptors/PacketWelcome/PacketWelcome.hpp
        src/RType/Components/Shared/Network.hpp
        src/Network/Packets/Descriptors/PacketPlayersData/PacketPlayersData.cpp
        src/Network/Packets/Descriptors/PacketPlayersData/PacketPlayersData.hpp
        src/Network/Packets/Descriptors/PacketEnemiesData/PacketEnemiesData.cpp
        src/Network/Packets/Descriptors/PacketEnemiesData/PacketEnemiesData.hpp
        src/RType/Models/Scene.hpp
        src/RType/Components/Shared/GameState.hpp
        src/ECS/Scene/SceneManager.hpp
        src/ECS/Scene/SceneManager.hpp
        src/RType/Scenes/Menu/Menu.cpp
        src/RType/Scenes/Menu/Menu.hpp
        src/ECS/Scene/AScene.hpp
        src/RType/Scenes/Game/Game.cpp
        src/RType/Scenes/Game/Game.hpp
        src/RType/Scenes/Lose/Lose.cpp
        src/RType/Scenes/Lose/Lose.hpp
        src/RType/Scenes/Win/Win.cpp
        src/RType/Scenes/Win/Win.hpp
        src/RType/Entities/Game.cpp
        src/RType/Entities/Game.hpp
        src/Network/Packets/Descriptors/PacketStartGame/PacketStartGame.cpp
        src/Network/Packets/Descriptors/PacketStartGame/PacketStartGame.hpp
        src/Network/Packets/Descriptors/PacketPlayerCounter/PacketPlayerCounter.cpp
        src/Network/Packets/Descriptors/PacketPlayerCounter/PacketPlayerCounter.hpp
        src/RType/Entities/PlayerCounter.cpp
        src/RType/Entities/PlayerCounter.hpp
        src/RType/Components/Shared/Projectile.hpp
        src/Network/Packets/Descriptors/PacketPlayerShoot/PacketPlayerShoot.cpp
        src/Network/Packets/Descriptors/PacketPlayerShoot/PacketPlayerShoot.hpp
        src/RType/Systems/Network/handlers/INetworkHandler.hpp
        src/RType/Systems/Network/handlers/ANetworkHandler.hpp
        src/RType/Services/PlayerService/PlayerService.cpp
        src/RType/Services/PlayerService/PlayerService.hpp
        src/RType/Systems/Network/handlers/PlayerShootHandler/PlayerShootHandler.cpp
        src/RType/Systems/Network/handlers/PlayerShootHandler/PlayerShootHandler.hpp
        src/RType/Services/ProjectileService/ProjectileService.cpp
        src/RType/Services/ProjectileService/ProjectileService.hpp
        src/RType/Systems/Network/handlers/ConnectHandler/ConnectHandler.cpp
        src/RType/Systems/Network/handlers/ConnectHandler/ConnectHandler.hpp
        src/RType/Components/Shared/MenuState.hpp
        src/RType/Systems/Network/handlers/StartGameHandler/StartGameHandler.cpp
        src/RType/Systems/Network/handlers/StartGameHandler/StartGameHandler.hpp
        src/RType/Systems/Network/handlers/EnnemiesDataHandler/EnnemiesDataHandler.cpp
        src/RType/Systems/Network/handlers/EnnemiesDataHandler/EnnemiesDataHandler.hpp
        src/RType/Systems/Network/handlers/PlayerDataHandler/PlayerDataHandler.cpp
        src/RType/Systems/Network/handlers/PlayerDataHandler/PlayerDataHandler.hpp
        src/Network/Packets/Descriptors/PacketEndGame/PacketEndGame.cpp
        src/Network/Packets/Descriptors/PacketEndGame/PacketEndGame.hpp
        src/RType/Models/EEnemyType.hpp
        src/RType/Levels/Level.hpp
        src/RType/Models/Spawn.hpp
        src/RType/Levels/LevelBuilder.hpp
        src/RType/Levels/LevelManager.cpp
        src/RType/Levels/LevelManager.hpp
        src/RType/Levels/Level.cpp
        src/RType/Systems/LevelRunner.cpp
        src/RType/Systems/LevelRunner.hpp
        src/RType/Services/EnemyService/EnemyService.cpp
        src/RType/Services/EnemyService/EnemyService.hpp
        src/Network/Packets/Descriptors/PacketLevelsRegistered/PacketLevelsRegistered.cpp
        src/Network/Packets/Descriptors/PacketLevelsRegistered/PacketLevelsRegistered.hpp
        src/RType/Models/EBonusType.hpp
        src/RType/Components/Shared/Bonus.hpp
        src/RType/Services/BonusService/BonusService.cpp
        src/RType/Services/BonusService/BonusService.hpp
        src/Network/Packets/Descriptors/PacketBonus/PacketBonus.cpp
        src/Network/Packets/Descriptors/PacketBonus/PacketBonus.hpp
        src/RType/Components/Shared/EventId.hpp
        src/RType/Models/BonusData.hpp
        src/RType/Models/ProjectileData.hpp
        src/RType/Components/Shared/PlayerBonuses.hpp
        src/RType/Entities/BossTurret.cpp
        src/RType/Entities/BossTurret.hpp
        src/RType/Components/Shared/ParentEntity.hpp
        src/RType/Components/Shared/ChildEntities.hpp
        src/RType/Components/Server/AiShoot.hpp
        src/RType/Entities/EnemyCornus.cpp
        src/RType/Entities/EnemyCornus.hpp
        src/RType/Entities/EnemyCrabus.cpp
        src/RType/Entities/EnemyCrabus.hpp
        src/RType/Entities/EnemyPablitos.cpp
        src/RType/Entities/EnemyPablitos.hpp
        src/RType/Entities/BossHeart.cpp
        src/RType/Entities/BossHeart.hpp
        src/RType/Entities/BossStaros.cpp
        src/RType/Entities/BossStaros.hpp
        src/RType/Entities/BossAlien.cpp
        src/RType/Entities/BossAlien.hpp
)

set(CLIENT_SRC
        src/RType/Systems/Network/handlers/EndGameHandler/EndGameHandler.cpp
        src/RType/Systems/Network/handlers/EndGameHandler/EndGameHandler.hpp
        src/RType/Systems/Network/handlers/LevelsRegisteredHandler/LevelsRegisteredHandler.cpp
        src/RType/Systems/Network/handlers/LevelsRegisteredHandler/LevelsRegisteredHandler.hpp
        src/RType/Systems/Network/handlers/PlayerCountHandler/PlayerCountHandler.cpp
        src/RType/Systems/Network/handlers/PlayerCountHandler/PlayerCountHandler.hpp
        src/RType/Systems/Network/handlers/WelcomeHandler/WelcomeHandler.hpp
        src/RType/Systems/Network/handlers/WelcomeHandler/WelcomeHandler.cpp
        src/RType/Entities/Window.cpp
        src/RType/Entities/Window.hpp
        src/RType/Systems/RenderWindow.cpp
        src/RType/Systems/RenderWindow.hpp
        src/RType/Systems/InputSystem.cpp
        src/RType/Systems/InputSystem.hpp
        include/ThreadPool.hpp
        src/Network/Packets/IPacket.hpp
        include/Networks.hpp
        src/RType/Components/Client/Sprite.hpp
        src/RType/Components/Client/Sound.hpp
        src/RType/Components/Client/Music.hpp
        src/RType/Models/PlayerData.hpp
        src/RType/Components/Shared/NetId.hpp
        src/RType/Components/Shared/ActualPlayer.hpp
        src/RType/Components/Shared/Speed.hpp
        src/RType/Components/Shared/Damage.hpp
        src/RType/Models/EnemyData.hpp
        src/ECS/Scene/IScene.hpp
        src/RType/Entities/Image.cpp
        src/RType/Entities/Image.hpp
        src/ECS/IEntity.hpp
        src/ECS/AEntity.hpp
        src/RType/Components/Client/OnClick.hpp
        src/RType/Entities/Button.cpp
        src/RType/Entities/Button.hpp
        src/RType/Components/Client/SfText.hpp
        src/RType/Components/Shared/Counter.hpp
        src/RType/Components/Client/SlidingBg.hpp
        src/RType/Systems/AnimationProjectile.cpp
        src/RType/Systems/AnimationProjectile.hpp
        src/RType/Components/Shared/NoDamageToPlayer.hpp
        src/RType/Systems/Sound/Sound.cpp
        src/RType/Systems/Sound/Sound.hpp
        src/RType/TextureManager/TextureManager.cpp
        src/RType/TextureManager/TextureManager.hpp
        src/RType/Systems/Network/handlers/BonusHandler/BonusHandler.cpp
        src/RType/Systems/Network/handlers/BonusHandler/BonusHandler.hpp
)


set(RTYPE_HEADERS
        include/Components.hpp
        include/ECS.hpp
        include/Systems.hpp
        include/Packets.hpp
        include/Networks.hpp
        include/Keys.hpp
        src/RType/Components/Shared/ProjectileInfo.hpp
)

link_libraries(inih::inireader spdlog::spdlog asio::asio nlohmann_json::nlohmann_json)

if(WIN32)
    add_compile_definitions(_WIN32_WINNT=0x0A00)
endif()

#region Server
add_executable(r-type_server ${RTYPE_SRC} ${RTYPE_HEADERS})

target_compile_definitions(r-type_server PRIVATE RTYPE_IS_SERVER)
#endregion

#region Client
add_executable(r-type_client ${RTYPE_SRC} ${CLIENT_SRC} ${RTYPE_HEADERS})

target_compile_definitions(r-type_client PRIVATE RTYPE_IS_CLIENT)

target_link_libraries(r-type_client sfml-system sfml-window sfml-graphics sfml-audio)
#endregion
