cmake_minimum_required(VERSION 3.28)

project(RType VERSION 1.0.0 LANGUAGES CXX)

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
        src/RType/Systems/Network.cpp
        src/RType/Systems/Network.hpp
        src/ThreadPool/ThreadPool.cpp
        src/Network/Packets/PacketFactory/PacketFactory.cpp
        src/Network/Packets/PacketFactory/PacketFactory.hpp
        src/Network/Packets/Descriptors/PacketConnect/PacketConnect.cpp
        src/Network/Packets/Descriptors/PacketWelcome/PacketWelcome.cpp
        src/Network/Packets/Descriptors/PacketWelcome/PacketWelcome.hpp
        src/Network/Packets/Descriptors/PacketNewPlayer/PacketNewPlayer.cpp
        src/Network/Packets/Descriptors/PacketNewPlayer/PacketNewPlayer.hpp
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
        src/RType/Systems/MonsterSpawner.cpp
        src/RType/Systems/MonsterSpawner.hpp
)

set(CLIENT_SRC
        src/RType/Entities/Window.cpp
        src/RType/Entities/Window.hpp
        src/RType/Systems/RenderWindow.cpp
        src/RType/Systems/RenderWindow.hpp
        src/RType/Systems/InputSystem.cpp
        src/RType/Systems/InputSystem.hpp
        src/Network/UDPNetwork/UDPNetwork.cpp
        src/Network/UDPNetwork/UDPNetwork.hpp
        src/RType/Systems/Network.cpp
        src/RType/Systems/Network.hpp
        src/ThreadPool/ThreadPool.cpp
        include/ThreadPool.hpp
        src/Network/TCPNetwork/TCPNetwork.cpp
        src/Network/TCPNetwork/TCPNetwork.hpp
        src/Network/TCPNetwork/TCPNetwork.cpp
        src/Network/Packets/IPacket.hpp
        src/Network/Packets/PacketFactory/PacketFactory.cpp
        src/Network/Packets/PacketFactory/PacketFactory.hpp
        src/Network/Packets/Descriptors/PacketConnect/PacketConnect.cpp
        include/Networks.hpp
        src/RType/Components/Client/Sprite.hpp
        src/Network/Packets/Descriptors/PacketWelcome/PacketWelcome.cpp
        src/Network/Packets/Descriptors/PacketWelcome/PacketWelcome.hpp
        src/Network/Packets/Descriptors/PacketNewPlayer/PacketNewPlayer.cpp
        src/Network/Packets/Descriptors/PacketNewPlayer/PacketNewPlayer.hpp
        src/RType/Components/Shared/Network.hpp
        src/Network/Packets/Descriptors/PacketPlayersData/PacketPlayersData.cpp
        src/Network/Packets/Descriptors/PacketPlayersData/PacketPlayersData.hpp
        src/RType/Models/PlayerData.hpp
        src/RType/Components/Shared/NetId.hpp
        src/RType/Components/Shared/ActualPlayer.hpp
        src/RType/Components/Shared/Speed.hpp
        src/RType/Components/Shared/Damage.hpp
        src/Network/Packets/Descriptors/PacketEnemiesData/PacketEnemiesData.cpp
        src/Network/Packets/Descriptors/PacketEnemiesData/PacketEnemiesData.hpp
        src/RType/Models/EnemyData.hpp
        src/RType/Models/Scene.hpp
        src/RType/Components/Shared/GameState.hpp
        src/ECS/Scene/SceneManager.hpp
        src/ECS/Scene/SceneManager.hpp
        src/ECS/Scene/IScene.hpp
        src/RType/Scenes/Menu/Menu.cpp
        src/RType/Scenes/Menu/Menu.hpp
        src/ECS/Scene/AScene.hpp
        src/RType/Entities/Image.cpp
        src/RType/Entities/Image.hpp
        src/ECS/IEntity.hpp
        src/ECS/AEntity.hpp
        src/ECS/AEntity.hpp
        src/RType/Scenes/Game/Game.cpp
        src/RType/Scenes/Game/Game.hpp
        src/RType/Components/Client/OnClick.hpp
        src/RType/Entities/Button.cpp
        src/RType/Entities/Button.hpp
        src/RType/Components/Client/SfText.hpp
        src/RType/Components/Shared/Counter.hpp
        src/RType/Entities/Game.cpp
        src/RType/Entities/Game.hpp
        src/Network/Packets/Descriptors/PacketStartGame/PacketStartGame.cpp
        src/Network/Packets/Descriptors/PacketStartGame/PacketStartGame.hpp
        src/RType/Components/Client/SlidingBg.hpp
        src/RType/Systems/AnimationProjectile.cpp
        src/RType/Systems/AnimationProjectile.hpp
        src/RType/Components/Shared/NoDamageToPlayer.hpp
)

set(RTYPE_HEADERS
        include/Components.hpp
        include/ECS.hpp
        include/Systems.hpp
        include/Packets.hpp
        include/Networks.hpp
)

link_libraries(inih::inireader spdlog::spdlog asio::asio)

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
