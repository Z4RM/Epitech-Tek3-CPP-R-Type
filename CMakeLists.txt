cmake_minimum_required(VERSION 3.28)

project(RType VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

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
)

set(CLIENT_SRC
        src/RType/Client/Client.cpp
        src/RType/Client/Client.hpp
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
)

set(RTYPE_HEADERS
        include/Components.hpp
        include/ECS.hpp
        include/Systems.hpp
        include/Packets.hpp
        include/Networks.hpp
)

link_libraries(inih::inireader spdlog::spdlog asio::asio)

#region Server
add_executable(r-type_server ${RTYPE_SRC} ${RTYPE_HEADERS})

target_compile_definitions(r-type_server PRIVATE RTYPE_IS_SERVER)
#endregion

#region Client
add_executable(r-type_client ${RTYPE_SRC} ${CLIENT_SRC} ${RTYPE_HEADERS})

target_compile_definitions(r-type_client PRIVATE RTYPE_IS_CLIENT)

target_link_libraries(r-type_client sfml-system sfml-window sfml-graphics sfml-audio)
#endregion
