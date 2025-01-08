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
    class IComponent {
    public:
        virtual ~IComponent() = default;
        virtual void create(nlohmann::basic_json<>& value) = 0;
    };
}



#endif //ICOMPONENT_HPP
