/*
** EPITECH PROJECT, 2025
** RType
** File description:
** FrameLimit.hpp
*/


#pragma once
#include <SFML/Graphics.hpp>
#include "IComponent.hpp"

namespace rtype::components {
    struct FrameLimit : public IComponent
    {
        void create(nlohmann::basic_json<> &value) override {
            limit = value;
        }
        int limit;
    };
}
