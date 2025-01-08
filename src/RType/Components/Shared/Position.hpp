/*
** EPITECH PROJECT, 2025
** RType
** File description:
** TODO: add description
*/

#pragma once
#include "IComponent.hpp"

namespace rtype::components {
    struct Position : public IComponent
    {
        void create(nlohmann::basic_json<>& value) override {
            x = value["x"];
            y = value["y"];
            z = value["z"];
        };
        float x;
        float y;
        float z;
    };
}
