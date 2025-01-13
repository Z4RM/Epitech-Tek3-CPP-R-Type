/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Speed.hpp
*/


#pragma once
#include "IComponent.hpp"

namespace rtype::components {
    struct Speed : IComponent
    {
        void create(nlohmann::basic_json<> &value) override {
            this->value = value;
        }
        int value;
    };
}
