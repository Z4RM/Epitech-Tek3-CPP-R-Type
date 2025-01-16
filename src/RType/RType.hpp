/*
** EPITECH PROJECT, 2024
** RType
** File description:
** RType.hpp
*/

#ifndef RTYPE_RTYPE_HPP_
#define RTYPE_RTYPE_HPP_

#include "GameLoader.hpp"
#include "ECS.hpp"
#ifdef RTYPE_IS_CLIENT
#include "Client/Client.hpp"
#endif

namespace rtype {
    /**
     * @class RType
     *
     * @brief The main class of the R-Type, containing, among others, the entrypoint.
     */
    class RType {
    public:
        /**
         * @brief The entrypoint of the R-Type.
         * Should be the only statement in the program main function, used as is:
         * ```cpp
         * #include "RType/RType.hpp"
         *
         * int main() {
         *     return rtype::RType::run();
         * }
         * ```
         *
         * @return The exit status of the program.
         */
        static int run();

        /**
         * @brief Creates components for entities based on the configuration file.
         * This function iterates through the global configuration, creates entities,
         * and assigns components to them using the specified data in the configuration.
         * Each component is created using the corresponding creator function from
         * the component map and registered with the entity and component managers.
         *
         * @param gameLoader The loader responsible for reading the game configuration.
         * @param rtype The identifier for the current RType instance.
         * @param entityManager The entity manager responsible for managing entities.
         * @param componentManager The component manager responsible for managing components.
         */
        static void createComponentViaConfig(GameLoader &gameLoader, size_t rtype, ecs::EntityManager &entityManager, ecs::ComponentManager &componentManager);

#ifdef RTYPE_IS_CLIENT
        // TODO: documentation
        void startServer();

        // TODO: documentation
        void stopServer();
#endif

    private:
        // TODO: documentation
        explicit RType(unsigned short port);

        // TODO: documentation
        int _run();

        // TODO: documentation
        bool _running();

        // TODO: documentation
        unsigned short _port;

#ifdef RTYPE_IS_CLIENT
        // TODO: documentation
        Client _client;
#endif
    };
}

#endif /* !RTYPE_RTYPE_HPP_ */
