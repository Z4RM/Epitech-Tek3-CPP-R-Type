/*
** EPITECH PROJECT, 2025
** RType
** File description:
** TODO: add description
*/

#pragma once
#include "IComponent.hpp"

namespace rtype::components {
    struct Size : public IComponent
    {
        void create(nlohmann::basic_json<> &value) override {
            width = value["width"];
            height = value["height"];
        }
        float width;
        float height;
    };
}