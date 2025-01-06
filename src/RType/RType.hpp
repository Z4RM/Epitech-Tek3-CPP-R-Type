/*
** EPITECH PROJECT, 2024
** RType
** File description:
** RType.hpp
*/

#ifndef RTYPE_RTYPE_HPP_
#define RTYPE_RTYPE_HPP_

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
