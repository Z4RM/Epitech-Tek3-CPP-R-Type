/*
** EPITECH PROJECT, 2024
** RType
** File description:
** RType.hpp
*/

#ifndef RTYPE_RTYPE_HPP_
#define RTYPE_RTYPE_HPP_

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
#else
        /** @brief load json levels in the assets/levels folder **/
        static void loadLevels();
#endif
    };
}

#endif /* !RTYPE_RTYPE_HPP_ */
