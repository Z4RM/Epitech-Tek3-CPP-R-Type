cmake_minimum_required(VERSION 3.29)
project(RType VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

find_package(spdlog REQUIRED)
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

include_directories(include src)

set(RTYPE_SRC
        src/main.cpp
        src/ECS/ComponentManager.hpp
        src/ECS/EntityManager.hpp
        src/ECS/SparseSet.hpp
        src/ECS/SystemManager.hpp
        src/RType/ModeManager/ModeManager.cpp
        src/RType/ModeManager/ModeManager.hpp
        src/RType/Game/Game.cpp
        src/RType/Game/Game.hpp
)

set(RTYPE_HEADERS
        include/foo.hpp
)

#region Server
add_executable(r-type_server ${RTYPE_SRC} ${RTYPE_HEADERS})

target_compile_definitions(r-type_server PRIVATE RTYPE_IS_SERVER)

target_link_libraries(r-type_server PRIVATE spdlog::spdlog sfml-system sfml-network)
#endregion

#region Client
add_executable(r-type_client ${RTYPE_SRC} ${RTYPE_HEADERS})

target_compile_definitions(r-type_client PRIVATE RTYPE_IS_CLIENT)

target_link_libraries(r-type_client PRIVATE spdlog::spdlog sfml-system sfml-window sfml-graphics sfml-network sfml-audio)
#endregion
