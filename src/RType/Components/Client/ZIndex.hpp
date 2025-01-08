/*
** EPITECH PROJECT, 2025
** RType
** File description:
** TODO: add description
*/

#pragma once
#include "IComponent.hpp"

namespace rtype::components {
    struct ZIndex : IComponent
    {
        void create(nlohmann::basic_json<> &value) override {
            this->value = value;
        }
        int value;
    };
}