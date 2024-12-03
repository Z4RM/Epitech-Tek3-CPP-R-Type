cmake_minimum_required(VERSION 3.29)
project(RType VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

find_package(spdlog REQUIRED)

set(RTYPE_SRC
        src/main.cpp
        src/ECS/ComponentManager.hpp
        src/ECS/EntityManager.hpp
        src/ECS/SparseSet.hpp
        src/ECS/SystemManager.hpp
)

set(RTYPE_HEADERS
        include/foo.hpp
)

#region Client
add_executable(r-type_client ${RTYPE_SRC} ${RTYPE_HEADERS})

target_compile_definitions(r-type_client PRIVATE RTYPE_IS_CLIENT)

target_include_directories(r-type_client PUBLIC include)

target_link_libraries(r-type_client PRIVATE spdlog::spdlog)
#endregion

#region Server
add_executable(r-type_server ${RTYPE_SRC} ${RTYPE_HEADERS})

target_compile_definitions(r-type_server PRIVATE RTYPE_IS_SERVER)

target_include_directories(r-type_server PUBLIC include)

target_link_libraries(r-type_server PRIVATE spdlog::spdlog)
#endregion
