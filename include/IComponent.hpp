/*
** EPITECH PROJECT, 2025
** RType
** File description:
** IComponent.hpp
*/


#ifndef ICOMPONENT_HPP
#define ICOMPONENT_HPP

#include <nlohmann/json.hpp>

namespace rtype::components {
    /**
     * @brief Base interface for all components in the R-Type game engine.
     *
     * The `IComponent` interface provides a common structure for all game components,
     * ensuring that each component can be initialized from a JSON object.
     * Derived classes must implement the `create` method to define how the component
     * should be populated from the provided JSON data.
     */
    class IComponent {
    public:
        /**
         * @brief Virtual destructor to ensure proper cleanup of derived classes.
         */
        virtual ~IComponent() = default;

        /**
         * @brief Pure virtual method for initializing a component with JSON data.
         *
         * This method must be implemented by derived classes to define how the component
         * is created or populated based on the provided JSON object.
         *
         * @param value A reference to a JSON object containing the initialization data.
         */
        virtual void create(nlohmann::basic_json<>& value) = 0;
    };
}



#endif //ICOMPONENT_HPP
