cmake_minimum_required(VERSION 3.29)

project(RType VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

find_package(spdlog REQUIRED)
find_package(SFML 2.6 COMPONENTS system window graphics network audio REQUIRED)

include_directories(include src)

set(RTYPE_SRC
        src/main.cpp
        src/ECS/ComponentManager.hpp
        src/ECS/EntityManager.hpp
        src/ECS/SparseSet.hpp
        src/ECS/SystemManager.hpp
        src/Components/Player.cpp
        src/Components/Player.hpp
        src/RType/ModeManager/ModeManager.cpp
        src/RType/ModeManager/ModeManager.hpp
)

set(RTYPE_HEADERS
        include/Structure.hpp
        include/ECS.hpp
)

#region Server
add_executable(r-type_server ${RTYPE_SRC} ${RTYPE_HEADERS})

target_compile_definitions(r-type_server PRIVATE RTYPE_IS_SERVER)

target_link_libraries(r-type_server PRIVATE spdlog::spdlog)
#endregion

#region Client
add_executable(r-type_client ${RTYPE_SRC} ${RTYPE_HEADERS})

target_compile_definitions(r-type_client PRIVATE RTYPE_IS_CLIENT)

target_link_libraries(r-type_client PRIVATE spdlog::spdlog sfml-system sfml-window sfml-graphics sfml-audio)
#endregion
